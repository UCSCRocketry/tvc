"""
rocket_grapher.py
=================

Purpose
-------
Utility module for visualizing 3D rocket flight data logged to CSV files.
Designed for simulation and flight-analysis workflows where state vectors
(position, velocity, forces, attitude) are recorded per timestep.

Key features
------------
- World-frame trajectory plotting
- Vector-field overlays (velocity, net force, etc.)
- Body-frame orientation visualization via quaternions (body -> world)
- Time-based decimation (decimate_hz) so you can log fast but plot sanely
- Optional world Z flip (flip_z) for NED(+Z down) vs ENU(+Z up)-style visuals
- Selectable body axes, with per-axis length control

Expected CSV Schema
-------------------
The CSV file is expected to contain (at minimum) the following columns:

    time
    pos_w_x, pos_w_y, pos_w_z
    vel_w_x, vel_w_y, vel_w_z
    ang_vel_b_x, ang_vel_b_y, ang_vel_b_z
    quat_b_to_w_w, quat_b_to_w_x, quat_b_to_w_y, quat_b_to_w_z
    F_net_w_x, F_net_w_y, F_net_w_z
    Tau_b_x, Tau_b_y, Tau_b_z

Units are not enforced, but consistency is assumed.

Primary Interface
-----------------
    graph3d(filepath, **kwargs)

Author / Credit
---------------
Written with assistance from ChatGPT (OpenAI), December 2025.
Structure, math conventions, and plotting design tailored for rocket
simulation and control-system development workflows.

You are encouraged to modify, extend, and adapt this file freely.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Optional, Sequence, Tuple, Union, Mapping

import math
import numpy as np
import matplotlib.pyplot as plt


# ---------------------------
# Column schema you described
# ---------------------------
REQUIRED_COLUMNS = [
    "time",
    "pos_w_x", "pos_w_y", "pos_w_z",
    "vel_w_x", "vel_w_y", "vel_w_z",
    "ang_vel_b_x", "ang_vel_b_y", "ang_vel_b_z",
    "quat_b_to_w_w", "quat_b_to_w_x", "quat_b_to_w_y", "quat_b_to_w_z",
    "F_net_w_x", "F_net_w_y", "F_net_w_z",
    "Tau_b_x", "Tau_b_y", "Tau_b_z",
]


def _as_path(filepath: Union[str, Path]) -> Path:
    if isinstance(filepath, Path):
        p = filepath
    elif isinstance(filepath, str):
        p = Path(filepath)
    else:
        raise TypeError(f"filepath must be str or Path, got {type(filepath).__name__}")

    if p.suffix.lower() != ".csv":
        raise ValueError(f"Expected a .csv file, got: {p.name}")
    if not p.exists():
        raise FileNotFoundError(f"File not found: {p}")
    if not p.is_file():
        raise ValueError(f"Not a file: {p}")
    return p


def _load_csv(filepath: Path) -> Dict[str, np.ndarray]:
    """
    Load CSV into dict of numpy arrays keyed by column name.
    Tries pandas first (more robust), falls back to csv module.
    """
    # Try pandas
    try:
        import pandas as pd  # type: ignore
        df = pd.read_csv(filepath)
        data: Dict[str, np.ndarray] = {}
        for c in df.columns:
            data[str(c).strip()] = pd.to_numeric(df[c], errors="coerce").to_numpy(dtype=float)
        return data
    except Exception:
        pass

    # Fallback: csv module
    import csv
    with filepath.open("r", newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        if reader.fieldnames is None:
            raise ValueError("CSV has no header row.")
        cols = [c.strip() for c in reader.fieldnames]
        rows = list(reader)

    data = {c: [] for c in cols}
    for row in rows:
        for c in cols:
            val = row.get(c, "")
            try:
                data[c].append(float(val))
            except Exception:
                data[c].append(float("nan"))

    return {c: np.asarray(v, dtype=float) for c, v in data.items()}


def _require_columns(data: Dict[str, np.ndarray], required: Sequence[str]) -> None:
    missing = [c for c in required if c not in data]
    if missing:
        raise ValueError(f"Missing required columns: {missing}")


def _get_vec3(data: Dict[str, np.ndarray], prefix: str) -> np.ndarray:
    """
    prefix like 'pos_w' -> returns Nx3 array from pos_w_x, pos_w_y, pos_w_z
    """
    x = data[f"{prefix}_x"]
    y = data[f"{prefix}_y"]
    z = data[f"{prefix}_z"]
    if not (len(x) == len(y) == len(z)):
        raise ValueError(f"Column lengths mismatch for {prefix}_x/y/z")
    return np.column_stack([x, y, z])


def _get_quat_wxyz(data: Dict[str, np.ndarray], prefix: str) -> np.ndarray:
    """
    prefix like 'quat_b_to_w' -> returns Nx4 (w, x, y, z)
    """
    w = data[f"{prefix}_w"]
    x = data[f"{prefix}_x"]
    y = data[f"{prefix}_y"]
    z = data[f"{prefix}_z"]
    if not (len(w) == len(x) == len(y) == len(z)):
        raise ValueError(f"Column lengths mismatch for {prefix}_w/x/y/z")
    return np.column_stack([w, x, y, z])


def _normalize_vectors(v: np.ndarray, eps: float = 1e-12) -> np.ndarray:
    norms = np.linalg.norm(v, axis=1, keepdims=True)
    norms = np.maximum(norms, eps)
    return v / norms


def _quat_to_rotmat_wxyz(q: np.ndarray) -> np.ndarray:
    """
    q: (4,) array [w, x, y, z]
    returns 3x3 rotation matrix (body -> world)

    Convention:
    - Hamilton quaternion (w + xi + yj + zk)
    - Right-handed rotation
    """
    w, x, y, z = float(q[0]), float(q[1]), float(q[2]), float(q[3])
    n = math.sqrt(w*w + x*x + y*y + z*z)
    if n == 0.0 or not math.isfinite(n):
        return np.eye(3)
    w, x, y, z = w/n, x/n, y/n, z/n

    return np.array([
        [1 - 2*(y*y + z*z),     2*(x*y - z*w),     2*(x*z + y*w)],
        [    2*(x*y + z*w), 1 - 2*(x*x + z*z),     2*(y*z - x*w)],
        [    2*(x*z - y*w),     2*(y*z + x*w), 1 - 2*(x*x + y*y)],
    ], dtype=float)


def _set_axes_equal_3d(ax, x: np.ndarray, y: np.ndarray, z: np.ndarray) -> None:
    """
    Make 3D plot have equal scale (best-effort).
    """
    x_min, x_max = np.nanmin(x), np.nanmax(x)
    y_min, y_max = np.nanmin(y), np.nanmax(y)
    z_min, z_max = np.nanmin(z), np.nanmax(z)

    cx = 0.5 * (x_min + x_max)
    cy = 0.5 * (y_min + y_max)
    cz = 0.5 * (z_min + z_max)

    r = max(x_max - x_min, y_max - y_min, z_max - z_min) * 0.5
    if not np.isfinite(r) or r == 0:
        r = 1.0

    ax.set_xlim(cx - r, cx + r)
    ax.set_ylim(cy - r, cy + r)
    ax.set_zlim(cz - r, cz + r)


def _decimate_indices_by_hz(time_s: np.ndarray, decimate_hz: float) -> np.ndarray:
    """
    Time-based decimation: keep points so the plotted sample rate is <= decimate_hz.

    Keeps the first finite sample, then greedily keeps the next sample whose time
    is >= last_kept_time + (1/decimate_hz). Works with variable-rate logging.

    Args:
        time_s: 1D time array in seconds
        decimate_hz: maximum kept samples per second (must be > 0)

    Returns:
        1D int array of indices to keep
    """
    if decimate_hz <= 0:
        raise ValueError("decimate_hz must be > 0")

    n = len(time_s)
    if n == 0:
        return np.array([], dtype=int)
    if n == 1:
        return np.array([0], dtype=int)

    t = np.asarray(time_s, dtype=float)
    dt_min = 1.0 / float(decimate_hz)

    i0 = None
    for i in range(n):
        if np.isfinite(t[i]):
            i0 = i
            break
    if i0 is None:
        return np.array([0], dtype=int)

    keep = [i0]
    last_t = t[i0]

    for i in range(i0 + 1, n):
        if not np.isfinite(t[i]):
            continue
        if t[i] - last_t >= dt_min:
            keep.append(i)
            last_t = t[i]

    return np.asarray(keep, dtype=int)


def _validate_body_axes_lengths(
    body_axes: Tuple[str, ...],
    body_axes_lengths: Optional[Mapping[str, float]],
    default_len: float,
) -> Dict[str, float]:
    """
    Returns dict axis->length for axes in body_axes.
    """
    axes_set = {a.lower() for a in body_axes}
    valid_axes = {"x", "y", "z"}
    if not axes_set.issubset(valid_axes):
        raise ValueError(f"body_axes must be subset of {valid_axes}, got {body_axes}")

    lengths: Dict[str, float] = {a: float(default_len) for a in axes_set}

    if body_axes_lengths is not None:
        for k, v in body_axes_lengths.items():
            kk = str(k).lower()
            if kk not in valid_axes:
                raise ValueError(f"Invalid axis in body_axes_lengths: {k!r}. Use 'x','y','z'.")
            if kk in lengths:
                vv = float(v)
                if vv < 0:
                    raise ValueError(f"body_axes_lengths[{k!r}] must be >= 0, got {vv}")
                lengths[kk] = vv

    return lengths


def graph3d(
    filepath: Union[str, Path],
    *,
    # decimation
    decimate_hz: float = 25.0,

    # coordinate display transform
    flip_z: bool = False,  # if True: world z -> -z for display (pos + world vectors + body axes)

    # what to draw
    show_trajectory: bool = True,
    show_velocity: bool = True,
    show_force: bool = False,

    # body axes selection + lengths
    body_axes: Optional[Tuple[str, ...]] = None,                 # e.g. ("z",) or ("x","z")
    body_axes_lengths: Optional[Mapping[str, float]] = None,     # e.g. {"x":0.2,"z":0.6}
    body_axes_default_length: float = 1.0,
    body_axes_stride: int = 5,                                   # applied on top of time-decimation

    # vector thinning & scaling
    vec_stride: int = 2,            # extra thinning on top of time-decimation, for quivers
    normalize_vectors: bool = True,
    vec_length: float = 1.0,

    # plot settings
    title: Optional[str] = None,
    show: bool = True,
) -> Tuple[plt.Figure, "plt.Axes"]:
    """
    3D plot of rocket data with time-based decimation (decimate_hz).

    Body axes:
        body_axes=("x","z") controls which axes are drawn.
        body_axes_lengths lets you set each axis length independently.
        Any axis not listed in body_axes_lengths uses body_axes_default_length.

    Example:
        graph3d(..., body_axes=("x","z"), body_axes_lengths={"x":0.2,"z":0.8})
    """
    p = _as_path(filepath)
    data = _load_csv(p)
    _require_columns(data, REQUIRED_COLUMNS)

    # Extract signals
    t = data["time"]
    pos = _get_vec3(data, "pos_w")
    vel = _get_vec3(data, "vel_w")
    F = _get_vec3(data, "F_net_w")
    quat = _get_quat_wxyz(data, "quat_b_to_w")

    n = pos.shape[0]
    if n < 2:
        raise ValueError("Not enough samples to plot (need at least 2 rows).")

    if not isinstance(vec_stride, int) or vec_stride <= 0:
        raise ValueError(f"vec_stride must be a positive int, got {vec_stride}")
    if not isinstance(body_axes_stride, int) or body_axes_stride <= 0:
        raise ValueError(f"body_axes_stride must be a positive int, got {body_axes_stride}")
    if body_axes_default_length < 0:
        raise ValueError(f"body_axes_default_length must be >= 0, got {body_axes_default_length}")
    if vec_length < 0:
        raise ValueError(f"vec_length must be >= 0, got {vec_length}")

    # Optional world Z flip for display
    if flip_z:
        pos = pos.copy()
        vel = vel.copy()
        F = F.copy()
        pos[:, 2] *= -1.0
        vel[:, 2] *= -1.0
        F[:, 2] *= -1.0

    # Time-based decimation index
    base_idx = _decimate_indices_by_hz(t, decimate_hz=decimate_hz)

    # Trajectory uses base_idx; vector overlays are further thinned
    vec_idx = base_idx[::vec_stride]
    axes_idx = base_idx[::body_axes_stride]

    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")

    # Color management: pull distinct colors from Matplotlib's cycle
    cycle = plt.rcParams["axes.prop_cycle"].by_key().get("color", [])

    def next_color(i: int) -> str:
        if cycle:
            return cycle[i % len(cycle)]
        return "C0"

    layer = 0

    # Trajectory
    if show_trajectory:
        c = next_color(layer); layer += 1
        ax.plot(
            pos[base_idx, 0], pos[base_idx, 1], pos[base_idx, 2],
            label=f"trajectory (≤{decimate_hz:g} Hz)",
            color=c
        )

    # Velocity vectors
    if show_velocity:
        c = next_color(layer); layer += 1
        vv = vel[vec_idx].copy()
        if normalize_vectors:
            vv = _normalize_vectors(vv)
        ax.quiver(
            pos[vec_idx, 0], pos[vec_idx, 1], pos[vec_idx, 2],
            vv[:, 0], vv[:, 1], vv[:, 2],
            length=vec_length,
            normalize=False,
            label="vel_w",
            color=c,
            alpha=0.9,
        )

    # Net force vectors
    if show_force:
        c = next_color(layer); layer += 1
        ff = F[vec_idx].copy()
        if normalize_vectors:
            ff = _normalize_vectors(ff)
        ax.quiver(
            pos[vec_idx, 0], pos[vec_idx, 1], pos[vec_idx, 2],
            ff[:, 0], ff[:, 1], ff[:, 2],
            length=vec_length,
            normalize=False,
            label="F_net_w",
            color=c,
            alpha=0.9,
        )

    # Body axes selection + per-axis lengths
    if body_axes:
        lengths = _validate_body_axes_lengths(body_axes, body_axes_lengths, body_axes_default_length)

        # Body unit axes in body frame
        body_axes_map = {
            "x": (np.array([1.0, 0.0, 0.0]), "r", "body x-axis"),
            "y": (np.array([0.0, 1.0, 0.0]), "g", "body y-axis"),
            "z": (np.array([0.0, 0.0, 1.0]), "b", "body z-axis"),
        }

        axes_set = {a.lower() for a in body_axes}

        for i in axes_idx:
            R = _quat_to_rotmat_wxyz(quat[i])

            for axis_name in axes_set:
                axis_b, color, _label = body_axes_map[axis_name]
                axis_w = R @ axis_b

                if flip_z:
                    axis_w = axis_w.copy()
                    axis_w[2] *= -1.0

                L = lengths[axis_name]
                if L == 0:
                    continue

                ax.quiver(
                    pos[i, 0], pos[i, 1], pos[i, 2],
                    axis_w[0], axis_w[1], axis_w[2],
                    length=L,
                    color=color,
                    alpha=0.85,
                )

        # Add one legend entry per selected axis (no spam)
        for axis_name in axes_set:
            _, color, label = body_axes_map[axis_name]
            ax.plot([], [], [], color=color, label=f"{label} (L={lengths[axis_name]:g})")

    # Labels, title, legend
    ax.set_xlabel("X (world)")
    ax.set_ylabel("Y (world)")
    ax.set_zlabel("Z (world)" + (" (flipped)" if flip_z else ""))

    if title is None:
        title = f"Rocket 3D Graph: {p.name}"
    ax.set_title(title)

    _set_axes_equal_3d(ax, pos[base_idx, 0], pos[base_idx, 1], pos[base_idx, 2])
    ax.legend(loc="best")

    if show:
        plt.show()

    return fig, ax


# -------------------------
# Optional class wrapper
# -------------------------
@dataclass
class RocketGrapher:
    filepath: Union[str, Path]
    decimate_hz: float = 25.0
    flip_z: bool = False

    def plot(self, **kwargs):
        return graph3d(
            self.filepath,
            decimate_hz=self.decimate_hz,
            flip_z=self.flip_z,
            **kwargs
        )


if __name__ == "__main__":
    # Minimal CLI-ish usage:
    #   python rocket_grapher.py path/to/flight.csv
    import sys
    if len(sys.argv) < 2:
        raise SystemExit("Usage: python rocket_grapher.py path/to/flight.csv")

    graph3d(
        sys.argv[1],
        decimate_hz=25.0,
        flip_z=False,
        show_velocity=True,
        show_force=False,
        body_axes=("z",),
        body_axes_lengths={"z": 0.5},
        vec_length=0.4,
    )
