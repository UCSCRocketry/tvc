from __future__ import annotations
from abc import ABC, abstractmethod
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from rocket import Rocket

class Component(ABC):
    @abstractmethod
    def apply(self, rocket:Rocket):
        pass 

