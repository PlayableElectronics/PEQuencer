class Channel:
    """
    Channel interface class
    """

    def __init__(
        self,
        number: int,
        mute: bool = True,
        step: int = 16,
        limit: int = 16,
        bpm: int = 120,
        velocity: int = 177,
    ) -> None:
        self.number = number
        self.mute = mute
        self.step = step
        self.limit = limit
        self.bpm = bpm
        self.velocity = velocity

    def __repr__(self) -> str:
        return (
            f"number: {self.number}, "
            f"mute: {self.mute}, "
            f"step: {self.step}, "
            f"limit: {self.limit}, "
            f"bpm: {self.bpm}, "
            f"velocity: {self.velocity}"
        )

    def note_on(self) -> None:
        """
        Send note on signal via midi
        """
        pass

    def note_off(self) -> None:
        """
        Send note off signal via midi
        """
        pass
