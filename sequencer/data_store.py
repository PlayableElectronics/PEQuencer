def singleton(cls):
    instances = {}

    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]

    return get_instance


@singleton
class MenusContainer:
    def __init__(self):
        self._data = (
            ["STEP", "OFFSET", "LIMIT", "NOTE"],
            ["BPM", "GATE", "VELOCITY", "EACH"],
            ["SWING", "SIGN", "EMPTY", "EMPTY"],
            ["EMPTY", "EMPTY", "EMPTY", "EMPTY"],
        )  # Each page has four visible parameters

    def get_value(self, index):
        return self._data[index]


@singleton
class DisplayDataContainer:
    def __init__(self):
        self._data = {
            "page": 1,
            "menu_position": 1,
        }  # Current state of menu and option parameter

    def get_value(self, key):
        return self._data.get(key)

    def set_value(self, key, value):
        self._data[key] = value
