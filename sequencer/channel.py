class Channel:
    """
    Channel class interface
    """

    def __init__(
        self,
        number,
        mute: bool = False,
        steps: int = 16,
        limit: int = 16,
        velocity: int = 127,
        offset: int = 0,
    ) -> None:
        self.number = number
        self.mute = mute
        self.steps = steps
        self.limit = limit
        self.velocity = velocity
        self.offset = offset

    def __repr__(self) -> str:
        return (
            f"Number: {self.number}, "
            f"Mute: {self.mute}, "
            f"Steps: {self.steps}, "
            f"Limit: {self.limit}, "
            f"Velocity: {self.velocity}, "
            f"Offset: {self.offset}"
        )
