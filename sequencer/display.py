from .data_store import DisplayDataContainer, MenusContainer
import time


class Display:
    """
    Display interface
    """

    def __init__(self, hz_rate: int = 60, width: int = 128, height: int = 64) -> None:
        self.display_container = DisplayDataContainer()
        self.menus_container = MenusContainer()
        self.refresh_rate = 1 / hz_rate
        self.width = width
        self.height = height

    def run(self) -> None:
        """
        Should run as thread
        """
        while True:
            page = self.display_container.get_value("page")
            menu_position = self.display_container.get_value("menu_position")
            self.draw_page(page, menu_position)
            current_channel = 1  # mock, data flow nor prepared yet
            self.draw_visualization(current_channel)
            time.sleep(self.refresh_rate)

    def draw_page(self, page, menu_position) -> None:
        """
        This method renders the page depending on the position and displays a
        rectangular cursor at the given parameter.
        """
        menu_strings = self.menus_container.get_value(page - 1)
        menu_strings_block_height = int(self.height / len(menu_strings))
        menu_strings_block_width = 50  # n px block width
        for text in menu_strings:
            # Draw x boxes
            if text == menu_position:
                # Draw selected block in oposite color
                # Draw selected text in oposite color
                ...
            # Draw unselected blocks
            # Draw unselected texts over blocks
            ...

    def draw_visualization(self, channel) -> None:
        """
        This method visualizes the sequence flow for each individual channel
        based on the parameters that have been set on it, such as the number
        of steps, limit, etc.
        """
        ...
