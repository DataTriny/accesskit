import accesskit
import os
import platform
import pygame

PLATFORM_SYSTEM = platform.system()

WINDOW_TITLE = "Hello world"
WINDOW_WIDTH = 400
WINDOW_HEIGHT = 200

WINDOW_ID = accesskit.NodeId(1)
BUTTON_1_ID = accesskit.NodeId(2)
BUTTON_2_ID = accesskit.NodeId(3)
ANNOUNCEMENT_ID = accesskit.NodeId(4)
INITIAL_FOCUS = BUTTON_1_ID

BUTTON_1_RECT = accesskit.Rect(20.0, 20.0, 100.0, 60.0)

BUTTON_2_RECT = accesskit.Rect(20.0, 60.0, 100.0, 100.0)

ACCESSKIT_EVENT = pygame.event.custom_type()
SET_FOCUS_EVENT = 0
DO_DEFAULT_ACTION_EVENT = 1

def build_button(id, name, classes):
    builder = accesskit.NodeBuilder(accesskit.Role.Button)
    builder.set_bounds(BUTTON_1_RECT if id == BUTTON_1_ID else BUTTON_2_RECT)
    builder.set_name(name)
    builder.add_action(accesskit.Action.Focus)
    builder.set_default_action_verb(accesskit.DefaultActionVerb.Click)
    return builder.build(classes)

def build_announcement(text, classes):
    builder = accesskit.NodeBuilder(accesskit.Role.StaticText)
    builder.set_name(text)
    builder.set_live(accesskit.Live.Polite)
    return builder.build(classes)

class PygameAdapter:
    def __init__(self, app_name, source, action_handler):
        if PLATFORM_SYSTEM == "Darwin":
            accesskit.macos.add_focus_forwarder_to_window_class("SDLWindow")
            window = pygame.display.get_wm_info()["window"]
            self.adapter = accesskit.macos.SubclassingAdapter(window, source, action_handler)
        elif os.name == "posix":
            self.adapter = accesskit.unix.Adapter.create(app_name, "pygame", "", source, action_handler)
        elif PLATFORM_SYSTEM == "Windows":
            hwnd = pygame.display.get_wm_info()["window"]
            self.adapter = accesskit.windows.SubclassingAdapter(hwnd, source, action_handler)

    def update(self, tree_update):
        if not self.adapter is None:
            events = self.adapter.update(tree_update)
            if not events is None:
                events.raise_events()

    def update_if_active(self, update_factory):
        if not self.adapter is None:
            if PLATFORM_SYSTEM in ["Darwin", "Windows"]:
                events = self.adapter.update_if_active(update_factory)
                if not events is None:
                    events.raise_events()
            else:
                self.adapter.update(update_factory())

    def update_root_window_bounds(self):
        # TODO: Figure out how to find window position.
        pass

class WindowState:
    def __init__(self):
        self._focus = INITIAL_FOCUS
        self.is_window_focused = False
        self.announcement = None
        self.node_classes = accesskit.NodeClassSet()

    def focus(self):
        return self._focus if self.is_window_focused else None

    def build_root(self):
        builder = accesskit.NodeBuilder(accesskit.Role.Window)
        builder.set_children([BUTTON_1_ID, BUTTON_2_ID])
        if not self.announcement is None:
            builder.push_child(ANNOUNCEMENT_ID)
        builder.set_name(WINDOW_TITLE)
        return builder.build(self.node_classes)

    def build_initial_tree(self):
        root = self.build_root()
        button_1 = build_button(BUTTON_1_ID, "Button 1", self.node_classes)
        button_2 = build_button(BUTTON_2_ID, "Button 2", self.node_classes)
        result = accesskit.TreeUpdate()
        result.tree = accesskit.Tree(WINDOW_ID)
        result.focus = self.focus()
        result.nodes.append((WINDOW_ID, root))
        result.nodes.append((BUTTON_1_ID, button_1))
        result.nodes.append((BUTTON_2_ID, button_2))
        if not self.announcement is None:
            result.nodes.append((ANNOUNCEMENT_ID, build_announcement(self.announcement, self.node_classes)))
        return result;

    def press_button(self, adapter, id):
        self.announcement = "You pressed button 1" if id == BUTTON_1_ID else "You pressed button 2"
        adapter.update_if_active(self.build_tree_update_for_button_press)

    def build_tree_update_for_button_press(self):
        update = accesskit.TreeUpdate()
        update.nodes.append((ANNOUNCEMENT_ID, build_announcement(self.announcement, self.node_classes)))
        update.nodes.append((WINDOW_ID, self.build_root()))
        update.focus = self.focus()
        return update

    def update_focus(self, adapter):
        adapter.update_if_active(self.build_tree_update_for_focus_update)

    def build_tree_update_for_focus_update(self):
        update = accesskit.TreeUpdate()
        update.focus = self.focus()
        return update

def do_action(request):
    if request.action in [accesskit.Action.Default, accesskit.Action.Focus]:
        args = {"event": SET_FOCUS_EVENT if request.action == accesskit.Action.Focus else DO_DEFAULT_ACTION_EVENT, "target": request.target}
        event = pygame.event.Event(ACCESSKIT_EVENT, args)
        pygame.event.post(event)

def main():
    print("This example has no visible GUI, and a keyboard interface:")
    print("- [Tab] switches focus between two logical buttons.")
    print("- [Space] 'presses' the button, adding static text in a live region announcing that it was pressed.")
    if PLATFORM_SYSTEM == "Windows":
        print("Enable Narrator with [Win]+[Ctrl]+[Enter] (or [Win]+[Enter] on older versions of Windows).")
    elif os.name == "posix" and PLATFORM_SYSTEM != "Darwin":
        print("Enable Orca with [Super]+[Alt]+[S].")

    state = WindowState()
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), pygame.HIDDEN)
    pygame.display.set_caption(WINDOW_TITLE)
    adapter = PygameAdapter("hello_world", state.build_initial_tree, do_action)
    pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), pygame.SHOWN)
    is_running = True
    while is_running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                is_running = False
            elif event.type == pygame.WINDOWFOCUSGAINED:
                state.is_window_focused = True
                state.update_focus(adapter)
            elif event.type == pygame.WINDOWFOCUSLOST:
                state.is_window_focused = False
                state.update_focus(adapter)
            elif event.type in [pygame.WINDOWMAXIMIZED, pygame.WINDOWMOVED, pygame.WINDOWRESIZED, pygame.WINDOWRESTORED, pygame.WINDOWSHOWN, pygame.WINDOWSIZECHANGED]:
                adapter.update_root_window_bounds()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_TAB:
                    state._focus = BUTTON_2_ID if state._focus == BUTTON_1_ID else BUTTON_1_ID
                    state.update_focus(adapter)
                elif event.key == pygame.K_SPACE:
                    state.press_button(adapter, state._focus)
            elif event.type == ACCESSKIT_EVENT and event.__dict__["target"] in [BUTTON_1_ID, BUTTON_2_ID]:
                if event.__dict__["event"] == SET_FOCUS_EVENT:
                    state._focus = event.__dict__["target"]
                    state.update_focus(adapter)
                elif event.__dict__["event"] == DO_DEFAULT_ACTION_EVENT:
                    state.press_button(adapter, event.__dict__["target"])

if __name__ == "__main__":
    main()
