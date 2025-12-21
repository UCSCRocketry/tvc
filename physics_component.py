from __future__ import annotations
from abc import ABC, abstractmethod
from typing import TYPE_CHECKING, Any
if TYPE_CHECKING:
    from rocket import Rocket

class Component(ABC):
    @abstractmethod
    def apply(self, rocket:Rocket, dt: float):
        pass 

    @abstractmethod
    def get_state_data(self) -> dict[str, Any]:
        #returns a dict in the form of [valuename, value]
        #can return empty dict
        pass

