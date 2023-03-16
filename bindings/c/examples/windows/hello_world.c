#include <stdio.h>
#include <windows.h>
#include "../../src/accesskit.h"

const char CLASS_NAME[] = "AccessKitTest";

const char WINDOW_TITLE[] = "Hello world";

static accesskit_node_id WINDOW_ID;
static accesskit_node_id BUTTON_1_ID;
static accesskit_node_id BUTTON_2_ID;
static accesskit_node_id ANNOUNCEMENT_ID;
static accesskit_node_id INITIAL_FOCUS;

const accesskit_rect BUTTON_1_RECT = { 20.0, 20.0, 100.0, 60.0 };

const accesskit_rect BUTTON_2_RECT = { 20.0, 60.0, 100.0, 100.0 };

const uint32_t SET_FOCUS_MSG = WM_USER;
const uint32_t DO_DEFAULT_ACTION_MSG = WM_USER + 1;

const bool node_id_cmp(const accesskit_node_id *id1, const accesskit_node_id *id2) {
    return memcmp(id1, id2, sizeof(accesskit_node_id)) == 0;
}

accesskit_node* build_button(accesskit_node_id id, char* name, accesskit_node_class_set *classes) {
    accesskit_rect rect;
    if (node_id_cmp(&id, &BUTTON_1_ID)) {
        rect = BUTTON_1_RECT;
    } else {
        rect = BUTTON_2_RECT;
    }

    accesskit_node_builder *builder = accesskit_node_builder_new(ACCESSKIT_ROLE_BUTTON);
    accesskit_node_builder_set_bounds(builder, rect);
    accesskit_node_builder_set_name(builder, name);
    accesskit_node_builder_add_action(builder, ACCESSKIT_ACTION_FOCUS);
    accesskit_node_builder_set_default_action_verb(builder, ACCESSKIT_DEFAULT_ACTION_VERB_CLICK);
    return accesskit_node_builder_build(builder, classes);
}

accesskit_node* build_announcement(char* text, accesskit_node_class_set *classes) {
    accesskit_node_builder *builder = accesskit_node_builder_new(ACCESSKIT_ROLE_STATIC_TEXT);
    accesskit_node_builder_set_name(builder, text);
    accesskit_node_builder_set_live(builder, ACCESSKIT_LIVE_POLITE);
    return accesskit_node_builder_build(builder, classes);
}

struct inner_window_state {
    accesskit_node_id focus;
    bool is_window_focused;
    char *announcement;
    accesskit_node_class_set *node_classes;
};

accesskit_opt_node_id inner_window_state_focus(struct inner_window_state *state) {
    accesskit_opt_node_id result;
    result.has_value = state->is_window_focused;
    if (result.has_value) {
        result.value = state->focus;
    }
    return result;
}

accesskit_node* inner_window_state_build_root(struct inner_window_state *state) {
    accesskit_node_builder *builder = accesskit_node_builder_new(ACCESSKIT_ROLE_WINDOW);
    accesskit_node_id *children = calloc(2, sizeof(accesskit_node_id));
    children[0] = BUTTON_1_ID;
    children[1] = BUTTON_2_ID;
    accesskit_node_builder_set_children(builder, 2, children);
    if (state->announcement != NULL) {
        accesskit_node_builder_push_child(builder, ANNOUNCEMENT_ID);
    }
    return accesskit_node_builder_build(builder, state->node_classes);
}

accesskit_tree_update inner_window_state_build_initial_tree(struct inner_window_state *state) {
    accesskit_node *root = inner_window_state_build_root(state);
    accesskit_node *button_1 = build_button(BUTTON_1_ID, "Button 1", state->node_classes);
    accesskit_node *button_2 = build_button(BUTTON_2_ID, "Button 2", state->node_classes);
    accesskit_tree_update result;
    result.tree.has_value = true;
    result.tree.value = accesskit_tree_new(WINDOW_ID);
    result.focus = inner_window_state_focus(state);
    int node_count = (state->announcement != NULL) ? 4 : 3;
    accesskit_node_id *ids = calloc(node_count, sizeof(accesskit_node_id));
    accesskit_node **nodes = calloc(node_count, sizeof(accesskit_node*));
    ids[0] = WINDOW_ID;
    nodes[0] = root;
    ids[1] = BUTTON_1_ID;
    nodes[1] = button_1;
    ids[2] = BUTTON_2_ID;
    nodes[2] = button_2;
    if (state->announcement != NULL) {
        ids[3] = ANNOUNCEMENT_ID;
        nodes[3] = build_announcement(state->announcement, state->node_classes);
    }
    result.nodes_length = node_count;
    result.ids = ids;
    result.nodes = nodes;
    return result;
}

struct window_state {
    accesskit_windows_uia_init_marker *uia_init_marker;
    accesskit_windows_adapter *adapter;
    struct inner_window_state *inner_state;
};

void window_state_free(struct window_state *state) {
    accesskit_windows_uia_init_marker_free(state->uia_init_marker);
    accesskit_windows_adapter_free(state->adapter);
    if (state->inner_state != NULL) {
        accesskit_node_class_set_free(state->inner_state->node_classes);
        free(state->inner_state);
    }
    free(state);
}

void do_action(const accesskit_action_request *request, void *userdata) {
    HWND window = userdata;
    LPARAM lparam = (LPARAM)&request->target;
    if (request->action == ACCESSKIT_ACTION_FOCUS) {
        PostMessage((HWND)window, SET_FOCUS_MSG, 0, lparam);
    } else if (request->action == ACCESSKIT_ACTION_DEFAULT) {
        PostMessage((HWND)window, DO_DEFAULT_ACTION_MSG, 0, lparam);
    }
}

accesskit_windows_adapter* window_state_get_or_init_accesskit_adapter(struct window_state *state, HWND window) {
    if (state->adapter != NULL) {
        return state->adapter;
    } else {
        struct inner_window_state *inner_state = state->inner_state;
        accesskit_tree_update initial_tree = inner_window_state_build_initial_tree(inner_state);
        accesskit_action_handler *action_handler = accesskit_action_handler_new(do_action, (void*)window);
        state->adapter = accesskit_windows_adapter_new(window, initial_tree, action_handler, state->uia_init_marker);
        return state->adapter;
    }
}

void window_state_press_button(struct window_state *state, accesskit_node_id id) {
    struct inner_window_state *inner_state = state->inner_state;
    char *text;
    if (node_id_cmp(&id, &BUTTON_1_ID)) {
        text = "You pressed button 1";
    } else {
        text = "You pressed button 2";
    }
    inner_state->announcement = text;
    if (state->adapter != NULL) {
        accesskit_node *announcement = build_announcement(text, inner_state->node_classes);
        accesskit_node *root = inner_window_state_build_root(inner_state);
        accesskit_tree_update update;
        update.nodes_length = 2;
        update.ids = calloc(update.nodes_length, sizeof(accesskit_node_id));
        update.nodes = calloc(update.nodes_length, sizeof(accesskit_node*));
        update.ids[0] = ANNOUNCEMENT_ID;
        update.nodes[0] = announcement;
        update.ids[1] = WINDOW_ID;
        update.nodes[1] = root;
        update.tree.has_value = false;
        update.focus = inner_window_state_focus(inner_state);
        accesskit_windows_queued_events *events = accesskit_windows_adapter_update(state->adapter, update);
        free(update.ids);
        free(update.nodes);
        accesskit_windows_queued_events_raise(events);
    }
}

struct window_state* get_window_state(HWND window) {
    return (struct window_state*)(GetWindowLongPtr(window, GWLP_USERDATA));
}

void update_focus(HWND window, bool is_window_focused) {
    const struct window_state *window_state = get_window_state(window);
    struct inner_window_state *inner_state = window_state->inner_state;
    inner_state->is_window_focused = is_window_focused;
    accesskit_opt_node_id focus = inner_window_state_focus(inner_state);
    if (window_state->adapter != NULL) {
        accesskit_tree_update update;
        update.nodes_length = 0;
        update.tree.has_value = false;
        update.focus = focus;
        accesskit_windows_queued_events *events = accesskit_windows_adapter_update(window_state->adapter, update);
        accesskit_windows_queued_events_raise(events);
    }
}

struct window_create_params {
    accesskit_node_id initial_focus;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        CREATESTRUCT *create_struct = (CREATESTRUCT*)lParam;
        struct window_create_params *create_params = (struct window_create_params*)create_struct->lpCreateParams;
        struct inner_window_state *inner_state = malloc(sizeof(struct inner_window_state));
        inner_state->focus = create_params->initial_focus;
        inner_state->is_window_focused = false;
        inner_state->announcement = NULL;
        inner_state->node_classes = accesskit_node_class_set_new();
        struct window_state *state = malloc(sizeof(struct window_state));
        state->uia_init_marker = accesskit_windows_uia_init_marker_new();
        state->adapter = NULL;
        state->inner_state = inner_state;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)state);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    } else if (msg == WM_PAINT) {
        ValidateRect(hwnd, NULL);
    } else if (msg == WM_DESTROY) {
        LONG_PTR ptr = SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        if (ptr != 0) {
            struct window_state *state = (struct window_state*)ptr;
            window_state_free(state);
        }
        PostQuitMessage(0);
    } else if (msg == WM_GETOBJECT) {
        struct window_state *window_state = get_window_state(hwnd);
        if (window_state == NULL) {
            // We need to be prepared to gracefully handle WM_GETOBJECT
            // while the window is being destroyed; this can happen if
            // the thread is using a COM STA.
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        accesskit_windows_adapter *adapter = window_state_get_or_init_accesskit_adapter(window_state, hwnd);
        accesskit_opt_lresult result = accesskit_windows_adapter_handle_wm_getobject(adapter, wParam, lParam);
        if (result.has_value) {
            return result.value;
        } else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    } else if (msg == WM_SETFOCUS || msg == WM_EXITMENULOOP || msg == WM_EXITSIZEMOVE) {
        update_focus(hwnd, true);
    } else if (msg == WM_KILLFOCUS || msg == WM_ENTERMENULOOP || msg == WM_ENTERSIZEMOVE) {
        update_focus(hwnd, false);
    } else if (msg == WM_KEYDOWN) {
        if (wParam == VK_TAB) {
            struct window_state *window_state = get_window_state(hwnd);
            struct inner_window_state *inner_state = window_state->inner_state;
            if (node_id_cmp(&inner_state->focus, &BUTTON_1_ID)) {
                inner_state->focus = BUTTON_2_ID;
            } else {
                inner_state->focus = BUTTON_1_ID;
            }
            update_focus(hwnd, true);
        } else if (wParam == VK_SPACE) {
            struct window_state *window_state = get_window_state(hwnd);
            accesskit_node_id id = window_state->inner_state->focus;
            window_state_press_button(window_state, id);
        } else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    } else if (msg == SET_FOCUS_MSG) {
        accesskit_node_id *id = (accesskit_node_id*)lParam;
        if (node_id_cmp(id, &BUTTON_1_ID) || node_id_cmp(id, &BUTTON_2_ID)) {
            struct window_state *window_state = get_window_state(hwnd);
            struct inner_window_state *inner_state = window_state->inner_state;
            inner_state->focus = *id;
            bool is_window_focused = inner_state->is_window_focused;
            update_focus(hwnd, is_window_focused);
        }
    } else if (msg == DO_DEFAULT_ACTION_MSG) {
        accesskit_node_id *id = (accesskit_node_id*)lParam;
        if (node_id_cmp(id, &BUTTON_1_ID) || node_id_cmp(id, &BUTTON_2_ID)) {
            struct window_state *window_state = get_window_state(hwnd);
            window_state_press_button(window_state, *id);
        }
    } else {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

HWND create_window(const char *title, accesskit_node_id initial_focus) {
    struct window_create_params *create_params = malloc(sizeof(struct window_create_params));
    create_params->initial_focus = initial_focus;

    return CreateWindowEx(
        WS_EX_CLIENTEDGE,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        create_params
    );
}

int main() {
    printf("This example has no visible GUI, and a keyboard interface:\n");
    printf("- [Tab] switches focus between two logical buttons.\n");
    printf("- [Space] 'presses' the button, adding static text in a live region announcing that it was pressed.\n");
    printf("Enable Narrator with [Win]+[Ctrl]+[Enter] (or [Win]+[Enter] on older versions of Windows).\n");

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        return 0;
    }

    WINDOW_ID = accesskit_node_id_new(1).value;
    BUTTON_1_ID = accesskit_node_id_new(2).value;
    BUTTON_2_ID = accesskit_node_id_new(3).value;
    ANNOUNCEMENT_ID = accesskit_node_id_new(4).value;
    INITIAL_FOCUS = BUTTON_1_ID;

    hwnd = create_window(WINDOW_TITLE, INITIAL_FOCUS);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
