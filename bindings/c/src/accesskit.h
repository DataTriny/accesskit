/**
 * Copyright 2023 The AccessKit Authors. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (found in
 * the LICENSE-APACHE file) or the MIT license (found in
 * the LICENSE-MIT file), at your option.
 */

#ifndef ACCESSKIT_H
#define ACCESSKIT_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <Foundation/NSObjCRuntime.h>
#endif

/**
 * An action to be taken on an accessibility node.
 *
 * In contrast to [`DefaultActionVerb`], these describe what happens to the
 * object, e.g. "focus".
 */
enum accesskit_action {
  /**
   * Do the default action for an object, typically this means "click".
   */
  ACCESSKIT_ACTION_DEFAULT,
  ACCESSKIT_ACTION_FOCUS,
  ACCESSKIT_ACTION_BLUR,
  ACCESSKIT_ACTION_COLLAPSE,
  ACCESSKIT_ACTION_EXPAND,
  /**
   * Requires [`ActionRequest::data`] to be set to [`ActionData::CustomAction`].
   */
  ACCESSKIT_ACTION_CUSTOM_ACTION,
  /**
   * Decrement a numeric value by one step.
   */
  ACCESSKIT_ACTION_DECREMENT,
  /**
   * Increment a numeric value by one step.
   */
  ACCESSKIT_ACTION_INCREMENT,
  ACCESSKIT_ACTION_HIDE_TOOLTIP,
  ACCESSKIT_ACTION_SHOW_TOOLTIP,
  /**
   * Request that the tree source invalidate its entire tree.
   */
  ACCESSKIT_ACTION_INVALIDATE_TREE,
  /**
   * Load inline text boxes for this subtree, providing information
   * about word boundaries, line layout, and individual character
   * bounding boxes.
   */
  ACCESSKIT_ACTION_LOAD_INLINE_TEXT_BOXES,
  /**
   * Delete any selected text in the control's text value and
   * insert the specified value in its place, like when typing or pasting.
   * Requires [`ActionRequest::data`] to be set to [`ActionData::Value`].
   */
  ACCESSKIT_ACTION_REPLACE_SELECTED_TEXT,
  ACCESSKIT_ACTION_SCROLL_BACKWARD,
  ACCESSKIT_ACTION_SCROLL_DOWN,
  ACCESSKIT_ACTION_SCROLL_FORWARD,
  ACCESSKIT_ACTION_SCROLL_LEFT,
  ACCESSKIT_ACTION_SCROLL_RIGHT,
  ACCESSKIT_ACTION_SCROLL_UP,
  /**
   * Scroll any scrollable containers to make the target object visible
   * on the screen.  Optionally set [`ActionRequest::data`] to
   * [`ActionData::ScrollTargetRect`].
   */
  ACCESSKIT_ACTION_SCROLL_INTO_VIEW,
  /**
   * Scroll the given object to a specified point in the tree's container
   * (e.g. window). Requires [`ActionRequest::data`] to be set to
   * [`ActionData::ScrollToPoint`].
   */
  ACCESSKIT_ACTION_SCROLL_TO_POINT,
  /**
   * Requires [`ActionRequest::data`] to be set to [`ActionData::SetScrollOffset`].
   */
  ACCESSKIT_ACTION_SET_SCROLL_OFFSET,
  /**
   * Requires [`ActionRequest::data`] to be set to [`ActionData::SetTextSelection`].
   */
  ACCESSKIT_ACTION_SET_TEXT_SELECTION,
  /**
   * Don't focus this node, but set it as the sequential focus navigation
   * starting point, so that pressing Tab moves to the next element
   * following this one, for example.
   */
  ACCESSKIT_ACTION_SET_SEQUENTIAL_FOCUS_NAVIGATION_STARTING_POINT,
  /**
   * Replace the value of the control with the specified value and
   * reset the selection, if applicable. Requires [`ActionRequest::data`]
   * to be set to [`ActionData::Value`] or [`ActionData::NumericValue`].
   */
  ACCESSKIT_ACTION_SET_VALUE,
  ACCESSKIT_ACTION_SHOW_CONTEXT_MENU,
};
typedef uint8_t accesskit_action;

enum accesskit_aria_current {
  ACCESSKIT_ARIA_CURRENT_FALSE,
  ACCESSKIT_ARIA_CURRENT_TRUE,
  ACCESSKIT_ARIA_CURRENT_PAGE,
  ACCESSKIT_ARIA_CURRENT_STEP,
  ACCESSKIT_ARIA_CURRENT_LOCATION,
  ACCESSKIT_ARIA_CURRENT_DATE,
  ACCESSKIT_ARIA_CURRENT_TIME,
};
typedef uint8_t accesskit_aria_current;

enum accesskit_checked_state {
  ACCESSKIT_CHECKED_STATE_FALSE,
  ACCESSKIT_CHECKED_STATE_TRUE,
  ACCESSKIT_CHECKED_STATE_MIXED,
};
typedef uint8_t accesskit_checked_state;

/**
 * Describes the action that will be performed on a given node when
 * executing the default action, which is a click.
 *
 * In contrast to [`Action`], these describe what the user can do on the
 * object, e.g. "press", not what happens to the object as a result.
 * Only one verb can be used at a time to describe the default action.
 */
enum accesskit_default_action_verb {
  ACCESSKIT_DEFAULT_ACTION_VERB_CLICK,
  ACCESSKIT_DEFAULT_ACTION_VERB_FOCUS,
  ACCESSKIT_DEFAULT_ACTION_VERB_CHECK,
  ACCESSKIT_DEFAULT_ACTION_VERB_UNCHECK,
  /**
   * A click will be performed on one of the node's ancestors.
   * This happens when the node itself is not clickable, but one of its
   * ancestors has click handlers attached which are able to capture the click
   * as it bubbles up.
   */
  ACCESSKIT_DEFAULT_ACTION_VERB_CLICK_ANCESTOR,
  ACCESSKIT_DEFAULT_ACTION_VERB_JUMP,
  ACCESSKIT_DEFAULT_ACTION_VERB_OPEN,
  ACCESSKIT_DEFAULT_ACTION_VERB_PRESS,
  ACCESSKIT_DEFAULT_ACTION_VERB_SELECT,
};
typedef uint8_t accesskit_default_action_verb;

enum accesskit_description_from {
  ACCESSKIT_DESCRIPTION_FROM_ARIA_DESCRIPTION,
  /**
   * HTML-AAM 5.2.2
   */
  ACCESSKIT_DESCRIPTION_FROM_BUTTON_LABEL,
  ACCESSKIT_DESCRIPTION_FROM_RELATED_ELEMENT,
  ACCESSKIT_DESCRIPTION_FROM_RUBY_ANNOTATION,
  /**
   * HTML-AAM 5.8.2
   */
  ACCESSKIT_DESCRIPTION_FROM_SUMMARY,
  /**
   * HTML-AAM 5.9.2
   */
  ACCESSKIT_DESCRIPTION_FROM_TABLE_CAPTION,
  ACCESSKIT_DESCRIPTION_FROM_TITLE,
};
typedef uint8_t accesskit_description_from;

enum accesskit_has_popup {
  ACCESSKIT_HAS_POPUP_TRUE,
  ACCESSKIT_HAS_POPUP_MENU,
  ACCESSKIT_HAS_POPUP_LISTBOX,
  ACCESSKIT_HAS_POPUP_TREE,
  ACCESSKIT_HAS_POPUP_GRID,
  ACCESSKIT_HAS_POPUP_DIALOG,
};
typedef uint8_t accesskit_has_popup;

/**
 * Indicates if a form control has invalid input or if a web DOM element has an
 * [`aria-invalid`] attribute.
 *
 * [`aria-invalid`]: https://www.w3.org/TR/wai-aria-1.1/#aria-invalid
 */
enum accesskit_invalid {
  ACCESSKIT_INVALID_TRUE,
  ACCESSKIT_INVALID_GRAMMAR,
  ACCESSKIT_INVALID_SPELLING,
};
typedef uint8_t accesskit_invalid;

enum accesskit_list_style {
  ACCESSKIT_LIST_STYLE_CIRCLE,
  ACCESSKIT_LIST_STYLE_DISC,
  ACCESSKIT_LIST_STYLE_IMAGE,
  ACCESSKIT_LIST_STYLE_NUMERIC,
  ACCESSKIT_LIST_STYLE_SQUARE,
  /**
   * Language specific ordering (alpha, roman, cjk-ideographic, etc...)
   */
  ACCESSKIT_LIST_STYLE_OTHER,
};
typedef uint8_t accesskit_list_style;

enum accesskit_live {
  ACCESSKIT_LIVE_OFF,
  ACCESSKIT_LIVE_POLITE,
  ACCESSKIT_LIVE_ASSERTIVE,
};
typedef uint8_t accesskit_live;

enum accesskit_name_from {
  /**
   * E.g. [`aria-label`].
   *
   * [`aria-label`]: https://www.w3.org/TR/wai-aria-1.1/#aria-label
   */
  ACCESSKIT_NAME_FROM_ATTRIBUTE,
  ACCESSKIT_NAME_FROM_ATTRIBUTE_EXPLICITLY_EMPTY,
  /**
   * E.g. in the case of a table, from a `caption` element.
   */
  ACCESSKIT_NAME_FROM_CAPTION,
  ACCESSKIT_NAME_FROM_CONTENTS,
  /**
   * E.g. from an HTML placeholder attribute on a text field.
   */
  ACCESSKIT_NAME_FROM_PLACEHOLDER,
  /**
   * E.g. from a `figcaption` element in a figure.
   */
  ACCESSKIT_NAME_FROM_RELATED_ELEMENT,
  /**
   * E.g. `<input type="text" title="title">`.
   */
  ACCESSKIT_NAME_FROM_TITLE,
  /**
   * E.g. `<input type="button" value="Button's name">`.
   */
  ACCESSKIT_NAME_FROM_VALUE,
};
typedef uint8_t accesskit_name_from;

enum accesskit_orientation {
  /**
   * E.g. most toolbars and separators.
   */
  ACCESSKIT_ORIENTATION_HORIZONTAL,
  /**
   * E.g. menu or combo box.
   */
  ACCESSKIT_ORIENTATION_VERTICAL,
};
typedef uint8_t accesskit_orientation;

/**
 * The type of an accessibility node.
 *
 * The majority of these roles come from the ARIA specification. Reference
 * the latest draft for proper usage.
 *
 * Like the AccessKit schema as a whole, this list is largely taken
 * from Chromium. However, unlike Chromium's alphabetized list, this list
 * is ordered roughly by expected usage frequency (with the notable exception
 * of [`Role::Unknown`]). This is more efficient in serialization formats
 * where integers use a variable-length encoding.
 */
enum accesskit_role {
  ACCESSKIT_ROLE_UNKNOWN,
  ACCESSKIT_ROLE_INLINE_TEXT_BOX,
  ACCESSKIT_ROLE_CELL,
  ACCESSKIT_ROLE_STATIC_TEXT,
  ACCESSKIT_ROLE_IMAGE,
  ACCESSKIT_ROLE_LINK,
  ACCESSKIT_ROLE_ROW,
  ACCESSKIT_ROLE_LIST_ITEM,
  /**
   * Contains the bullet, number, or other marker for a list item.
   */
  ACCESSKIT_ROLE_LIST_MARKER,
  ACCESSKIT_ROLE_TREE_ITEM,
  ACCESSKIT_ROLE_LIST_BOX_OPTION,
  ACCESSKIT_ROLE_MENU_ITEM,
  ACCESSKIT_ROLE_MENU_LIST_OPTION,
  ACCESSKIT_ROLE_PARAGRAPH,
  ACCESSKIT_ROLE_GENERIC_CONTAINER,
  /**
   * Used for ARIA role="none"/"presentation" -- ignored in platform tree.
   */
  ACCESSKIT_ROLE_PRESENTATION,
  ACCESSKIT_ROLE_CHECK_BOX,
  ACCESSKIT_ROLE_RADIO_BUTTON,
  ACCESSKIT_ROLE_TEXT_FIELD,
  ACCESSKIT_ROLE_BUTTON,
  ACCESSKIT_ROLE_LABEL_TEXT,
  ACCESSKIT_ROLE_PANE,
  ACCESSKIT_ROLE_ROW_HEADER,
  ACCESSKIT_ROLE_COLUMN_HEADER,
  ACCESSKIT_ROLE_COLUMN,
  ACCESSKIT_ROLE_ROW_GROUP,
  ACCESSKIT_ROLE_LIST,
  ACCESSKIT_ROLE_TABLE,
  ACCESSKIT_ROLE_TABLE_HEADER_CONTAINER,
  ACCESSKIT_ROLE_LAYOUT_TABLE_CELL,
  ACCESSKIT_ROLE_LAYOUT_TABLE_ROW,
  ACCESSKIT_ROLE_LAYOUT_TABLE,
  ACCESSKIT_ROLE_SWITCH,
  ACCESSKIT_ROLE_TOGGLE_BUTTON,
  ACCESSKIT_ROLE_MENU,
  ACCESSKIT_ROLE_ABBR,
  ACCESSKIT_ROLE_ALERT,
  ACCESSKIT_ROLE_ALERT_DIALOG,
  ACCESSKIT_ROLE_APPLICATION,
  ACCESSKIT_ROLE_ARTICLE,
  ACCESSKIT_ROLE_AUDIO,
  ACCESSKIT_ROLE_BANNER,
  ACCESSKIT_ROLE_BLOCKQUOTE,
  ACCESSKIT_ROLE_CANVAS,
  ACCESSKIT_ROLE_CAPTION,
  ACCESSKIT_ROLE_CARET,
  ACCESSKIT_ROLE_CLIENT,
  ACCESSKIT_ROLE_CODE,
  ACCESSKIT_ROLE_COLOR_WELL,
  ACCESSKIT_ROLE_COMBO_BOX_GROUPING,
  ACCESSKIT_ROLE_COMBO_BOX_MENU_BUTTON,
  ACCESSKIT_ROLE_COMPLEMENTARY,
  ACCESSKIT_ROLE_COMMENT,
  ACCESSKIT_ROLE_CONTENT_DELETION,
  ACCESSKIT_ROLE_CONTENT_INSERTION,
  ACCESSKIT_ROLE_CONTENT_INFO,
  ACCESSKIT_ROLE_DATE,
  ACCESSKIT_ROLE_DATE_TIME,
  ACCESSKIT_ROLE_DEFINITION,
  ACCESSKIT_ROLE_DESCRIPTION_LIST,
  ACCESSKIT_ROLE_DESCRIPTION_LIST_DETAIL,
  ACCESSKIT_ROLE_DESCRIPTION_LIST_TERM,
  ACCESSKIT_ROLE_DETAILS,
  ACCESSKIT_ROLE_DIALOG,
  ACCESSKIT_ROLE_DIRECTORY,
  ACCESSKIT_ROLE_DISCLOSURE_TRIANGLE,
  ACCESSKIT_ROLE_DOCUMENT,
  ACCESSKIT_ROLE_EMBEDDED_OBJECT,
  ACCESSKIT_ROLE_EMPHASIS,
  ACCESSKIT_ROLE_FEED,
  ACCESSKIT_ROLE_FIGURE_CAPTION,
  ACCESSKIT_ROLE_FIGURE,
  ACCESSKIT_ROLE_FOOTER,
  ACCESSKIT_ROLE_FOOTER_AS_NON_LANDMARK,
  ACCESSKIT_ROLE_FORM,
  ACCESSKIT_ROLE_GRID,
  ACCESSKIT_ROLE_GROUP,
  ACCESSKIT_ROLE_HEADER,
  ACCESSKIT_ROLE_HEADER_AS_NON_LANDMARK,
  ACCESSKIT_ROLE_HEADING,
  ACCESSKIT_ROLE_IFRAME,
  ACCESSKIT_ROLE_IFRAME_PRESENTATIONAL,
  ACCESSKIT_ROLE_IME_CANDIDATE,
  ACCESSKIT_ROLE_INPUT_TIME,
  ACCESSKIT_ROLE_KEYBOARD,
  ACCESSKIT_ROLE_LEGEND,
  ACCESSKIT_ROLE_LINE_BREAK,
  ACCESSKIT_ROLE_LIST_BOX,
  ACCESSKIT_ROLE_LOG,
  ACCESSKIT_ROLE_MAIN,
  ACCESSKIT_ROLE_MARK,
  ACCESSKIT_ROLE_MARQUEE,
  ACCESSKIT_ROLE_MATH,
  ACCESSKIT_ROLE_MENU_BAR,
  ACCESSKIT_ROLE_MENU_ITEM_CHECK_BOX,
  ACCESSKIT_ROLE_MENU_ITEM_RADIO,
  ACCESSKIT_ROLE_MENU_LIST_POPUP,
  ACCESSKIT_ROLE_METER,
  ACCESSKIT_ROLE_NAVIGATION,
  ACCESSKIT_ROLE_NOTE,
  ACCESSKIT_ROLE_PLUGIN_OBJECT,
  ACCESSKIT_ROLE_POPUP_BUTTON,
  ACCESSKIT_ROLE_PORTAL,
  ACCESSKIT_ROLE_PRE,
  ACCESSKIT_ROLE_PROGRESS_INDICATOR,
  ACCESSKIT_ROLE_RADIO_GROUP,
  ACCESSKIT_ROLE_REGION,
  ACCESSKIT_ROLE_ROOT_WEB_AREA,
  ACCESSKIT_ROLE_RUBY,
  ACCESSKIT_ROLE_RUBY_ANNOTATION,
  ACCESSKIT_ROLE_SCROLL_BAR,
  ACCESSKIT_ROLE_SCROLL_VIEW,
  ACCESSKIT_ROLE_SEARCH,
  ACCESSKIT_ROLE_SEARCH_BOX,
  ACCESSKIT_ROLE_SECTION,
  ACCESSKIT_ROLE_SLIDER,
  ACCESSKIT_ROLE_SPIN_BUTTON,
  ACCESSKIT_ROLE_SPLITTER,
  ACCESSKIT_ROLE_STATUS,
  ACCESSKIT_ROLE_STRONG,
  ACCESSKIT_ROLE_SUGGESTION,
  ACCESSKIT_ROLE_SVG_ROOT,
  ACCESSKIT_ROLE_TAB,
  ACCESSKIT_ROLE_TAB_LIST,
  ACCESSKIT_ROLE_TAB_PANEL,
  ACCESSKIT_ROLE_TERM,
  ACCESSKIT_ROLE_TEXT_FIELD_WITH_COMBO_BOX,
  ACCESSKIT_ROLE_TIME,
  ACCESSKIT_ROLE_TIMER,
  ACCESSKIT_ROLE_TITLE_BAR,
  ACCESSKIT_ROLE_TOOLBAR,
  ACCESSKIT_ROLE_TOOLTIP,
  ACCESSKIT_ROLE_TREE,
  ACCESSKIT_ROLE_TREE_GRID,
  ACCESSKIT_ROLE_VIDEO,
  ACCESSKIT_ROLE_WEB_VIEW,
  ACCESSKIT_ROLE_WINDOW,
  ACCESSKIT_ROLE_PDF_ACTIONABLE_HIGHLIGHT,
  ACCESSKIT_ROLE_PDF_ROOT,
  ACCESSKIT_ROLE_GRAPHICS_DOCUMENT,
  ACCESSKIT_ROLE_GRAPHICS_OBJECT,
  ACCESSKIT_ROLE_GRAPHICS_SYMBOL,
  ACCESSKIT_ROLE_DOC_ABSTRACT,
  ACCESSKIT_ROLE_DOC_ACKNOWLEDGEMENTS,
  ACCESSKIT_ROLE_DOC_AFTERWORD,
  ACCESSKIT_ROLE_DOC_APPENDIX,
  ACCESSKIT_ROLE_DOC_BACK_LINK,
  ACCESSKIT_ROLE_DOC_BIBLIO_ENTRY,
  ACCESSKIT_ROLE_DOC_BIBLIOGRAPHY,
  ACCESSKIT_ROLE_DOC_BIBLIO_REF,
  ACCESSKIT_ROLE_DOC_CHAPTER,
  ACCESSKIT_ROLE_DOC_COLOPHON,
  ACCESSKIT_ROLE_DOC_CONCLUSION,
  ACCESSKIT_ROLE_DOC_COVER,
  ACCESSKIT_ROLE_DOC_CREDIT,
  ACCESSKIT_ROLE_DOC_CREDITS,
  ACCESSKIT_ROLE_DOC_DEDICATION,
  ACCESSKIT_ROLE_DOC_ENDNOTE,
  ACCESSKIT_ROLE_DOC_ENDNOTES,
  ACCESSKIT_ROLE_DOC_EPIGRAPH,
  ACCESSKIT_ROLE_DOC_EPILOGUE,
  ACCESSKIT_ROLE_DOC_ERRATA,
  ACCESSKIT_ROLE_DOC_EXAMPLE,
  ACCESSKIT_ROLE_DOC_FOOTNOTE,
  ACCESSKIT_ROLE_DOC_FOREWORD,
  ACCESSKIT_ROLE_DOC_GLOSSARY,
  ACCESSKIT_ROLE_DOC_GLOSS_REF,
  ACCESSKIT_ROLE_DOC_INDEX,
  ACCESSKIT_ROLE_DOC_INTRODUCTION,
  ACCESSKIT_ROLE_DOC_NOTE_REF,
  ACCESSKIT_ROLE_DOC_NOTICE,
  ACCESSKIT_ROLE_DOC_PAGE_BREAK,
  ACCESSKIT_ROLE_DOC_PAGE_FOOTER,
  ACCESSKIT_ROLE_DOC_PAGE_HEADER,
  ACCESSKIT_ROLE_DOC_PAGE_LIST,
  ACCESSKIT_ROLE_DOC_PART,
  ACCESSKIT_ROLE_DOC_PREFACE,
  ACCESSKIT_ROLE_DOC_PROLOGUE,
  ACCESSKIT_ROLE_DOC_PULLQUOTE,
  ACCESSKIT_ROLE_DOC_QNA,
  ACCESSKIT_ROLE_DOC_SUBTITLE,
  ACCESSKIT_ROLE_DOC_TIP,
  ACCESSKIT_ROLE_DOC_TOC,
  /**
   * Behaves similar to an ARIA grid but is primarily used by Chromium's
   * `TableView` and its subclasses, so they can be exposed correctly
   * on certain platforms.
   */
  ACCESSKIT_ROLE_LIST_GRID,
};
typedef uint8_t accesskit_role;

enum accesskit_sort_direction {
  ACCESSKIT_SORT_DIRECTION_UNSORTED,
  ACCESSKIT_SORT_DIRECTION_ASCENDING,
  ACCESSKIT_SORT_DIRECTION_DESCENDING,
  ACCESSKIT_SORT_DIRECTION_OTHER,
};
typedef uint8_t accesskit_sort_direction;

enum accesskit_text_align {
  ACCESSKIT_TEXT_ALIGN_LEFT,
  ACCESSKIT_TEXT_ALIGN_RIGHT,
  ACCESSKIT_TEXT_ALIGN_CENTER,
  ACCESSKIT_TEXT_ALIGN_JUSTIFY,
};
typedef uint8_t accesskit_text_align;

enum accesskit_text_decoration {
  ACCESSKIT_TEXT_DECORATION_SOLID,
  ACCESSKIT_TEXT_DECORATION_DOTTED,
  ACCESSKIT_TEXT_DECORATION_DASHED,
  ACCESSKIT_TEXT_DECORATION_DOUBLE,
  ACCESSKIT_TEXT_DECORATION_WAVY,
};
typedef uint8_t accesskit_text_decoration;

enum accesskit_text_direction {
  ACCESSKIT_TEXT_DIRECTION_LEFT_TO_RIGHT,
  ACCESSKIT_TEXT_DIRECTION_RIGHT_TO_LEFT,
  ACCESSKIT_TEXT_DIRECTION_TOP_TO_BOTTOM,
  ACCESSKIT_TEXT_DIRECTION_BOTTOM_TO_TOP,
};
typedef uint8_t accesskit_text_direction;

enum accesskit_vertical_offset {
  ACCESSKIT_VERTICAL_OFFSET_SUBSCRIPT,
  ACCESSKIT_VERTICAL_OFFSET_SUPERSCRIPT,
};
typedef uint8_t accesskit_vertical_offset;

typedef struct accesskit_action_handler accesskit_action_handler;

#if defined(__APPLE__)
typedef struct accesskit_macos_adapter accesskit_macos_adapter;
#endif

#if defined(__APPLE__)
typedef struct accesskit_macos_queued_events accesskit_macos_queued_events;
#endif

#if defined(__APPLE__)
typedef struct accesskit_macos_subclassing_adapter accesskit_macos_subclassing_adapter;
#endif

typedef struct accesskit_node accesskit_node;

typedef struct accesskit_node_builder accesskit_node_builder;

typedef struct accesskit_node_class_set accesskit_node_class_set;

#if (defined(__linux__) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
typedef struct accesskit_unix_adapter accesskit_unix_adapter;
#endif

#if defined(_WIN32)
typedef struct accesskit_windows_adapter accesskit_windows_adapter;
#endif

#if defined(_WIN32)
typedef struct accesskit_windows_queued_events accesskit_windows_queued_events;
#endif

#if defined(_WIN32)
typedef struct accesskit_windows_subclassing_adapter accesskit_windows_subclassing_adapter;
#endif

#if defined(_WIN32)
typedef struct accesskit_windows_uia_init_marker accesskit_windows_uia_init_marker;
#endif

/**
 * Call `accesskit_node_id_new` to create this struct.
 *
 * If you have to manually populate this, ensure it is not filled up only with zeroes.
 */
typedef struct accesskit_node_id {
  uint8_t _0[16];
} accesskit_node_id;

typedef struct accesskit_node_ids {
  size_t length;
  const struct accesskit_node_id *values;
} accesskit_node_ids;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_node_id {
  bool has_value;
  struct accesskit_node_id value;
} accesskit_opt_node_id;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_double {
  bool has_value;
  double value;
} accesskit_opt_double;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_index {
  bool has_value;
  size_t value;
} accesskit_opt_index;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_color {
  bool has_value;
  uint32_t value;
} accesskit_opt_color;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_text_decoration {
  bool has_value;
  accesskit_text_decoration value;
} accesskit_opt_text_decoration;

typedef struct accesskit_lengths {
  size_t length;
  const uint8_t *values;
} accesskit_lengths;

typedef struct accesskit_opt_coords {
  bool has_value;
  size_t length;
  const float *values;
} accesskit_opt_coords;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_bool {
  bool has_value;
  bool value;
} accesskit_opt_bool;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_name_from {
  bool has_value;
  accesskit_name_from value;
} accesskit_opt_name_from;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_description_from {
  bool has_value;
  accesskit_description_from value;
} accesskit_opt_description_from;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_invalid {
  bool has_value;
  accesskit_invalid value;
} accesskit_opt_invalid;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_checked_state {
  bool has_value;
  accesskit_checked_state value;
} accesskit_opt_checked_state;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_live {
  bool has_value;
  accesskit_live value;
} accesskit_opt_live;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_default_action_verb {
  bool has_value;
  accesskit_default_action_verb value;
} accesskit_opt_default_action_verb;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_text_direction {
  bool has_value;
  accesskit_text_direction value;
} accesskit_opt_text_direction;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_orientation {
  bool has_value;
  accesskit_orientation value;
} accesskit_opt_orientation;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_sort_direction {
  bool has_value;
  accesskit_sort_direction value;
} accesskit_opt_sort_direction;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_aria_current {
  bool has_value;
  accesskit_aria_current value;
} accesskit_opt_aria_current;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_has_popup {
  bool has_value;
  accesskit_has_popup value;
} accesskit_opt_has_popup;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_list_style {
  bool has_value;
  accesskit_list_style value;
} accesskit_opt_list_style;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_text_align {
  bool has_value;
  accesskit_text_align value;
} accesskit_opt_text_align;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_vertical_offset {
  bool has_value;
  accesskit_vertical_offset value;
} accesskit_opt_vertical_offset;

/**
 * A 2D affine transform. Derived from [kurbo](https://github.com/linebender/kurbo).
 */
typedef struct accesskit_affine {
  double _0[6];
} accesskit_affine;

/**
 * A rectangle. Derived from [kurbo](https://github.com/linebender/kurbo).
 */
typedef struct accesskit_rect {
  /**
   * The minimum x coordinate (left edge).
   */
  double x0;
  /**
   * The minimum y coordinate (top edge in y-down spaces).
   */
  double y0;
  /**
   * The maximum x coordinate (right edge).
   */
  double x1;
  /**
   * The maximum y coordinate (bottom edge in y-down spaces).
   */
  double y1;
} accesskit_rect;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_rect {
  bool has_value;
  struct accesskit_rect value;
} accesskit_opt_rect;

typedef struct accesskit_text_position {
  struct accesskit_node_id node;
  size_t character_index;
} accesskit_text_position;

typedef struct accesskit_text_selection {
  struct accesskit_text_position anchor;
  struct accesskit_text_position focus;
} accesskit_text_selection;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_text_selection {
  bool has_value;
  struct accesskit_text_selection value;
} accesskit_opt_text_selection;

typedef struct accesskit_custom_action {
  int32_t id;
  char *description;
} accesskit_custom_action;

typedef struct accesskit_custom_actions {
  size_t length;
  const struct accesskit_custom_action *values;
} accesskit_custom_actions;

typedef struct accesskit_tree {
  struct accesskit_node_id root;
  struct accesskit_opt_node_id root_scroller;
} accesskit_tree;

/**
 * A 2D point. Derived from [kurbo](https://github.com/linebender/kurbo).
 */
typedef struct accesskit_point {
  /**
   * The x coordinate.
   */
  double x;
  /**
   * The y coordinate.
   */
  double y;
} accesskit_point;

typedef enum accesskit_action_data_Tag {
  ACCESSKIT_ACTION_DATA_CUSTOM_ACTION,
  ACCESSKIT_ACTION_DATA_VALUE,
  ACCESSKIT_ACTION_DATA_NUMERIC_VALUE,
  ACCESSKIT_ACTION_DATA_SCROLL_TARGET_RECT,
  ACCESSKIT_ACTION_DATA_SCROLL_TO_POINT,
  ACCESSKIT_ACTION_DATA_SET_SCROLL_OFFSET,
  ACCESSKIT_ACTION_DATA_SET_TEXT_SELECTION,
} accesskit_action_data_Tag;

typedef struct accesskit_action_data {
  accesskit_action_data_Tag tag;
  union {
    struct {
      int32_t custom_action;
    };
    struct {
      char *value;
    };
    struct {
      double numeric_value;
    };
    struct {
      struct accesskit_rect scroll_target_rect;
    };
    struct {
      struct accesskit_point scroll_to_point;
    };
    struct {
      struct accesskit_point set_scroll_offset;
    };
    struct {
      struct accesskit_text_selection set_text_selection;
    };
  };
} accesskit_action_data;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_action_data {
  bool has_value;
  struct accesskit_action_data value;
} accesskit_opt_action_data;

typedef struct accesskit_action_request {
  accesskit_action action;
  struct accesskit_node_id target;
  struct accesskit_opt_action_data data;
} accesskit_action_request;

typedef void (*accesskit_action_handler_callback)(const struct accesskit_action_request *request, void *userdata);

/**
 * A 2D vector. Derived from [kurbo](https://github.com/linebender/kurbo).
 *
 * This is intended primarily for a vector in the mathematical sense,
 * but it can be interpreted as a translation, and converted to and
 * from a point (vector relative to the origin) and size.
 */
typedef struct accesskit_vec2 {
  /**
   * The x-coordinate.
   */
  double x;
  /**
   * The y-coordinate.
   */
  double y;
} accesskit_vec2;

/**
 * A 2D size. Derived from [kurbo](https://github.com/linebender/kurbo).
 */
typedef struct accesskit_size {
  /**
   * The width.
   */
  double width;
  /**
   * The height.
   */
  double height;
} accesskit_size;

/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_tree {
  bool has_value;
  struct accesskit_tree value;
} accesskit_opt_tree;

/**
 * `ids` and `nodes` must point to the same number of elements, and this number must be given to `nodes_length`.
 * The caller is responsible for freeing `ids` and `nodes`.
 */
typedef struct accesskit_tree_update {
  size_t nodes_length;
  struct accesskit_node_id *ids;
  struct accesskit_node **nodes;
  struct accesskit_opt_tree tree;
  struct accesskit_opt_node_id focus;
} accesskit_tree_update;

#if defined(__APPLE__)
typedef struct accesskit_tree_update (*accesskit_tree_update_factory)(void*);
#endif

#if defined(_WIN32)
typedef struct accesskit_tree_update (*accesskit_tree_update_factory)(void*);
#endif

#if defined(_WIN32)
/**
 * Represents an optional value.
 *
 * If `has_value` is false, do not read the `value` field.
 */
typedef struct accesskit_opt_lresult {
  bool has_value;
  LRESULT value;
} accesskit_opt_lresult;
#endif

struct accesskit_node_class_set *accesskit_node_class_set_new(void);

void accesskit_node_class_set_free(struct accesskit_node_class_set *set);

void accesskit_node_free(struct accesskit_node *node);

accesskit_role accesskit_node_role(const struct accesskit_node *node);

accesskit_role accesskit_node_builder_role(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_role(struct accesskit_node_builder *builder, accesskit_role value);

bool accesskit_node_supports_action(const struct accesskit_node *node, accesskit_action action);

bool accesskit_node_builder_supports_action(const struct accesskit_node_builder *builder,
                                            accesskit_action action);

void accesskit_node_builder_add_action(struct accesskit_node_builder *builder,
                                       accesskit_action action);

void accesskit_node_builder_remove_action(struct accesskit_node_builder *builder,
                                          accesskit_action action);

void accesskit_node_builder_clear_actions(struct accesskit_node_builder *builder);

bool accesskit_node_is_autofill_available(const struct accesskit_node *node);

bool accesskit_node_is_default(const struct accesskit_node *node);

bool accesskit_node_is_editable(const struct accesskit_node *node);

bool accesskit_node_is_hovered(const struct accesskit_node *node);

bool accesskit_node_is_hidden(const struct accesskit_node *node);

bool accesskit_node_is_linked(const struct accesskit_node *node);

bool accesskit_node_is_multiline(const struct accesskit_node *node);

bool accesskit_node_is_multiselectable(const struct accesskit_node *node);

bool accesskit_node_is_protected(const struct accesskit_node *node);

bool accesskit_node_is_required(const struct accesskit_node *node);

bool accesskit_node_is_visited(const struct accesskit_node *node);

bool accesskit_node_is_busy(const struct accesskit_node *node);

bool accesskit_node_is_live_atomic(const struct accesskit_node *node);

bool accesskit_node_is_modal(const struct accesskit_node *node);

bool accesskit_node_is_scrollable(const struct accesskit_node *node);

bool accesskit_node_is_selected_from_focus(const struct accesskit_node *node);

bool accesskit_node_is_touch_pass_through(const struct accesskit_node *node);

bool accesskit_node_is_read_only(const struct accesskit_node *node);

bool accesskit_node_is_disabled(const struct accesskit_node *node);

bool accesskit_node_is_bold(const struct accesskit_node *node);

bool accesskit_node_is_italic(const struct accesskit_node *node);

bool accesskit_node_canvas_has_fallback(const struct accesskit_node *node);

bool accesskit_node_clips_children(const struct accesskit_node *node);

bool accesskit_node_is_line_breaking_object(const struct accesskit_node *node);

bool accesskit_node_is_page_breaking_object(const struct accesskit_node *node);

bool accesskit_node_is_spelling_error(const struct accesskit_node *node);

bool accesskit_node_is_grammar_error(const struct accesskit_node *node);

bool accesskit_node_is_search_match(const struct accesskit_node *node);

bool accesskit_node_is_suggestion(const struct accesskit_node *node);

bool accesskit_node_is_nonatomic_text_field_root(const struct accesskit_node *node);

bool accesskit_node_builder_is_autofill_available(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_autofill_available(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_autofill_available(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_default(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_default(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_default(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_editable(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_editable(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_editable(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_hovered(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_hovered(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_hovered(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_hidden(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_hidden(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_hidden(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_linked(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_linked(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_linked(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_multiline(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_multiline(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_multiline(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_multiselectable(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_multiselectable(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_multiselectable(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_protected(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_protected(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_protected(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_required(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_required(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_required(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_visited(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_visited(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_visited(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_busy(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_busy(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_busy(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_live_atomic(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_live_atomic(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_live_atomic(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_modal(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_modal(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_modal(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_scrollable(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scrollable(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_scrollable(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_selected_from_focus(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_selected_from_focus(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_selected_from_focus(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_touch_pass_through(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_touch_pass_through(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_touch_pass_through(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_read_only(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_read_only(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_read_only(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_disabled(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_disabled(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_disabled(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_bold(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_bold(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_bold(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_italic(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_italic(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_italic(struct accesskit_node_builder *builder);

bool accesskit_node_builder_canvas_has_fallback(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_canvas_has_fallback(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_canvas_has_fallback(struct accesskit_node_builder *builder);

bool accesskit_node_builder_clips_children(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_clips_children(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_clips_children(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_line_breaking_object(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_line_breaking_object(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_line_breaking_object(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_page_breaking_object(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_page_breaking_object(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_page_breaking_object(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_spelling_error(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_spelling_error(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_spelling_error(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_grammar_error(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_grammar_error(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_grammar_error(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_search_match(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_search_match(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_search_match(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_suggestion(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_suggestion(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_suggestion(struct accesskit_node_builder *builder);

bool accesskit_node_builder_is_nonatomic_text_field_root(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_is_nonatomic_text_field_root(struct accesskit_node_builder *builder);

void accesskit_node_builder_clear_is_nonatomic_text_field_root(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_children(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_children(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_children(struct accesskit_node_builder *builder,
                                         size_t length,
                                         struct accesskit_node_id *values);

void accesskit_node_builder_push_child(struct accesskit_node_builder *builder,
                                       struct accesskit_node_id item);

void accesskit_node_builder_clear_children(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_indirect_children(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_indirect_children(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_indirect_children(struct accesskit_node_builder *builder,
                                                  size_t length,
                                                  struct accesskit_node_id *values);

void accesskit_node_builder_push_indirect_child(struct accesskit_node_builder *builder,
                                                struct accesskit_node_id item);

void accesskit_node_builder_clear_indirect_children(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_controls(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_controls(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_controls(struct accesskit_node_builder *builder,
                                         size_t length,
                                         struct accesskit_node_id *values);

void accesskit_node_builder_push_controlled(struct accesskit_node_builder *builder,
                                            struct accesskit_node_id item);

void accesskit_node_builder_clear_controls(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_details(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_details(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_details(struct accesskit_node_builder *builder,
                                        size_t length,
                                        struct accesskit_node_id *values);

void accesskit_node_builder_push_detail(struct accesskit_node_builder *builder,
                                        struct accesskit_node_id item);

void accesskit_node_builder_clear_details(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_described_by(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_described_by(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_described_by(struct accesskit_node_builder *builder,
                                             size_t length,
                                             struct accesskit_node_id *values);

void accesskit_node_builder_push_described_by(struct accesskit_node_builder *builder,
                                              struct accesskit_node_id item);

void accesskit_node_builder_clear_described_by(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_flow_to(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_flow_to(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_flow_to(struct accesskit_node_builder *builder,
                                        size_t length,
                                        struct accesskit_node_id *values);

void accesskit_node_builder_push_flow_to(struct accesskit_node_builder *builder,
                                         struct accesskit_node_id item);

void accesskit_node_builder_clear_flow_to(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_labelled_by(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_labelled_by(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_labelled_by(struct accesskit_node_builder *builder,
                                            size_t length,
                                            struct accesskit_node_id *values);

void accesskit_node_builder_push_labelled_by(struct accesskit_node_builder *builder,
                                             struct accesskit_node_id item);

void accesskit_node_builder_clear_labelled_by(struct accesskit_node_builder *builder);

struct accesskit_node_ids accesskit_node_radio_group(const struct accesskit_node *node);

struct accesskit_node_ids accesskit_node_builder_radio_group(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_radio_group(struct accesskit_node_builder *builder,
                                            size_t length,
                                            struct accesskit_node_id *values);

void accesskit_node_builder_push_to_radio_group(struct accesskit_node_builder *builder,
                                                struct accesskit_node_id item);

void accesskit_node_builder_clear_radio_group(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_active_descendant(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_active_descendant(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_active_descendant(struct accesskit_node_builder *builder,
                                                  struct accesskit_node_id value);

void accesskit_node_builder_clear_active_descendant(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_error_message(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_error_message(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_error_message(struct accesskit_node_builder *builder,
                                              struct accesskit_node_id value);

void accesskit_node_builder_clear_error_message(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_in_page_link_target(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_in_page_link_target(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_in_page_link_target(struct accesskit_node_builder *builder,
                                                    struct accesskit_node_id value);

void accesskit_node_builder_clear_in_page_link_target(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_member_of(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_member_of(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_member_of(struct accesskit_node_builder *builder,
                                          struct accesskit_node_id value);

void accesskit_node_builder_clear_member_of(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_next_on_line(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_next_on_line(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_next_on_line(struct accesskit_node_builder *builder,
                                             struct accesskit_node_id value);

void accesskit_node_builder_clear_next_on_line(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_previous_on_line(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_previous_on_line(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_previous_on_line(struct accesskit_node_builder *builder,
                                                 struct accesskit_node_id value);

void accesskit_node_builder_clear_previous_on_line(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_popup_for(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_popup_for(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_popup_for(struct accesskit_node_builder *builder,
                                          struct accesskit_node_id value);

void accesskit_node_builder_clear_popup_for(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_table_header(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_table_header(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_header(struct accesskit_node_builder *builder,
                                             struct accesskit_node_id value);

void accesskit_node_builder_clear_table_header(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_table_row_header(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_table_row_header(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_row_header(struct accesskit_node_builder *builder,
                                                 struct accesskit_node_id value);

void accesskit_node_builder_clear_table_row_header(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_table_column_header(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_table_column_header(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_column_header(struct accesskit_node_builder *builder,
                                                    struct accesskit_node_id value);

void accesskit_node_builder_clear_table_column_header(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_next_focus(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_next_focus(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_next_focus(struct accesskit_node_builder *builder,
                                           struct accesskit_node_id value);

void accesskit_node_builder_clear_next_focus(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_previous_focus(const struct accesskit_node *node);

struct accesskit_opt_node_id accesskit_node_builder_previous_focus(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_previous_focus(struct accesskit_node_builder *builder,
                                               struct accesskit_node_id value);

void accesskit_node_builder_clear_previous_focus(struct accesskit_node_builder *builder);

/**
 * Only call this function with a string that originated from AccessKit.
 */
void accesskit_string_free(char *string);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_name(const struct accesskit_node *node);

char *accesskit_node_description(const struct accesskit_node *node);

char *accesskit_node_value(const struct accesskit_node *node);

char *accesskit_node_access_key(const struct accesskit_node *node);

char *accesskit_node_auto_complete(const struct accesskit_node *node);

char *accesskit_node_checked_state_description(const struct accesskit_node *node);

char *accesskit_node_class_name(const struct accesskit_node *node);

char *accesskit_node_css_display(const struct accesskit_node *node);

char *accesskit_node_font_family(const struct accesskit_node *node);

char *accesskit_node_html_tag(const struct accesskit_node *node);

char *accesskit_node_inner_html(const struct accesskit_node *node);

char *accesskit_node_input_type(const struct accesskit_node *node);

char *accesskit_node_key_shortcuts(const struct accesskit_node *node);

char *accesskit_node_language(const struct accesskit_node *node);

char *accesskit_node_live_relevant(const struct accesskit_node *node);

char *accesskit_node_placeholder(const struct accesskit_node *node);

char *accesskit_node_aria_role(const struct accesskit_node *node);

char *accesskit_node_role_description(const struct accesskit_node *node);

char *accesskit_node_tooltip(const struct accesskit_node *node);

char *accesskit_node_url(const struct accesskit_node *node);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_name(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_name(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_name(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_description(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_description(struct accesskit_node_builder *builder,
                                            const char *value);

void accesskit_node_builder_clear_description(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_value(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_value(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_value(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_access_key(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_access_key(struct accesskit_node_builder *builder,
                                           const char *value);

void accesskit_node_builder_clear_access_key(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_auto_complete(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_auto_complete(struct accesskit_node_builder *builder,
                                              const char *value);

void accesskit_node_builder_clear_auto_complete(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_checked_state_description(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_checked_state_description(struct accesskit_node_builder *builder,
                                                          const char *value);

void accesskit_node_builder_clear_checked_state_description(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_class_name(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_class_name(struct accesskit_node_builder *builder,
                                           const char *value);

void accesskit_node_builder_clear_class_name(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_css_display(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_css_display(struct accesskit_node_builder *builder,
                                            const char *value);

void accesskit_node_builder_clear_css_display(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_font_family(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_font_family(struct accesskit_node_builder *builder,
                                            const char *value);

void accesskit_node_builder_clear_font_family(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_html_tag(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_html_tag(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_html_tag(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_inner_html(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_inner_html(struct accesskit_node_builder *builder,
                                           const char *value);

void accesskit_node_builder_clear_inner_html(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_input_type(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_input_type(struct accesskit_node_builder *builder,
                                           const char *value);

void accesskit_node_builder_clear_input_type(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_key_shortcuts(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_key_shortcuts(struct accesskit_node_builder *builder,
                                              const char *value);

void accesskit_node_builder_clear_key_shortcuts(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_language(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_language(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_language(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_live_relevant(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_live_relevant(struct accesskit_node_builder *builder,
                                              const char *value);

void accesskit_node_builder_clear_live_relevant(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_placeholder(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_placeholder(struct accesskit_node_builder *builder,
                                            const char *value);

void accesskit_node_builder_clear_placeholder(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_aria_role(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_aria_role(struct accesskit_node_builder *builder,
                                          const char *value);

void accesskit_node_builder_clear_aria_role(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_role_description(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_role_description(struct accesskit_node_builder *builder,
                                                 const char *value);

void accesskit_node_builder_clear_role_description(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_tooltip(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_tooltip(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_tooltip(struct accesskit_node_builder *builder);

/**
 * Caller must call `accesskit_string_free` with the return value.
 */
char *accesskit_node_builder_url(const struct accesskit_node_builder *builder);

/**
 * Caller is responsible for freeing the memory pointed by `value`.
 */
void accesskit_node_builder_set_url(struct accesskit_node_builder *builder, const char *value);

void accesskit_node_builder_clear_url(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_x(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_x(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_x(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_x(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_x_min(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_x_min(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_x_min(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_x_min(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_x_max(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_x_max(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_x_max(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_x_max(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_y(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_y(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_y(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_y(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_y_min(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_y_min(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_y_min(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_y_min(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_scroll_y_max(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_scroll_y_max(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_scroll_y_max(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_scroll_y_max(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_numeric_value(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_numeric_value(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_numeric_value(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_numeric_value(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_min_numeric_value(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_min_numeric_value(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_min_numeric_value(struct accesskit_node_builder *builder,
                                                  double value);

void accesskit_node_builder_clear_min_numeric_value(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_max_numeric_value(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_max_numeric_value(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_max_numeric_value(struct accesskit_node_builder *builder,
                                                  double value);

void accesskit_node_builder_clear_max_numeric_value(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_numeric_value_step(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_numeric_value_step(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_numeric_value_step(struct accesskit_node_builder *builder,
                                                   double value);

void accesskit_node_builder_clear_numeric_value_step(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_numeric_value_jump(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_numeric_value_jump(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_numeric_value_jump(struct accesskit_node_builder *builder,
                                                   double value);

void accesskit_node_builder_clear_numeric_value_jump(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_font_size(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_font_size(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_font_size(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_font_size(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_font_weight(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_font_weight(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_font_weight(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_font_weight(struct accesskit_node_builder *builder);

struct accesskit_opt_double accesskit_node_text_indent(const struct accesskit_node *node);

struct accesskit_opt_double accesskit_node_builder_text_indent(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_text_indent(struct accesskit_node_builder *builder, double value);

void accesskit_node_builder_clear_text_indent(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_row_count(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_row_count(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_row_count(struct accesskit_node_builder *builder,
                                                size_t value);

void accesskit_node_builder_clear_table_row_count(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_column_count(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_column_count(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_column_count(struct accesskit_node_builder *builder,
                                                   size_t value);

void accesskit_node_builder_clear_table_column_count(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_row_index(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_row_index(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_row_index(struct accesskit_node_builder *builder,
                                                size_t value);

void accesskit_node_builder_clear_table_row_index(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_column_index(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_column_index(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_column_index(struct accesskit_node_builder *builder,
                                                   size_t value);

void accesskit_node_builder_clear_table_column_index(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_cell_column_index(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_cell_column_index(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_cell_column_index(struct accesskit_node_builder *builder,
                                                        size_t value);

void accesskit_node_builder_clear_table_cell_column_index(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_cell_column_span(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_cell_column_span(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_cell_column_span(struct accesskit_node_builder *builder,
                                                       size_t value);

void accesskit_node_builder_clear_table_cell_column_span(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_cell_row_index(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_cell_row_index(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_cell_row_index(struct accesskit_node_builder *builder,
                                                     size_t value);

void accesskit_node_builder_clear_table_cell_row_index(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_table_cell_row_span(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_table_cell_row_span(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_table_cell_row_span(struct accesskit_node_builder *builder,
                                                    size_t value);

void accesskit_node_builder_clear_table_cell_row_span(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_hierarchical_level(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_hierarchical_level(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_hierarchical_level(struct accesskit_node_builder *builder,
                                                   size_t value);

void accesskit_node_builder_clear_hierarchical_level(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_size_of_set(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_size_of_set(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_size_of_set(struct accesskit_node_builder *builder, size_t value);

void accesskit_node_builder_clear_size_of_set(struct accesskit_node_builder *builder);

struct accesskit_opt_index accesskit_node_position_in_set(const struct accesskit_node *node);

struct accesskit_opt_index accesskit_node_builder_position_in_set(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_position_in_set(struct accesskit_node_builder *builder,
                                                size_t value);

void accesskit_node_builder_clear_position_in_set(struct accesskit_node_builder *builder);

struct accesskit_opt_color accesskit_node_color_value(const struct accesskit_node *node);

struct accesskit_opt_color accesskit_node_builder_color_value(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_color_value(struct accesskit_node_builder *builder, uint32_t value);

void accesskit_node_builder_clear_color_value(struct accesskit_node_builder *builder);

struct accesskit_opt_color accesskit_node_background_color(const struct accesskit_node *node);

struct accesskit_opt_color accesskit_node_builder_background_color(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_background_color(struct accesskit_node_builder *builder,
                                                 uint32_t value);

void accesskit_node_builder_clear_background_color(struct accesskit_node_builder *builder);

struct accesskit_opt_color accesskit_node_foreground_color(const struct accesskit_node *node);

struct accesskit_opt_color accesskit_node_builder_foreground_color(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_foreground_color(struct accesskit_node_builder *builder,
                                                 uint32_t value);

void accesskit_node_builder_clear_foreground_color(struct accesskit_node_builder *builder);

struct accesskit_opt_text_decoration accesskit_node_overline(const struct accesskit_node *node);

struct accesskit_opt_text_decoration accesskit_node_builder_overline(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_overline(struct accesskit_node_builder *builder,
                                         accesskit_text_decoration value);

void accesskit_node_builder_clear_overline(struct accesskit_node_builder *builder);

struct accesskit_opt_text_decoration accesskit_node_strikethrough(const struct accesskit_node *node);

struct accesskit_opt_text_decoration accesskit_node_builder_strikethrough(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_strikethrough(struct accesskit_node_builder *builder,
                                              accesskit_text_decoration value);

void accesskit_node_builder_clear_strikethrough(struct accesskit_node_builder *builder);

struct accesskit_opt_text_decoration accesskit_node_underline(const struct accesskit_node *node);

struct accesskit_opt_text_decoration accesskit_node_builder_underline(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_underline(struct accesskit_node_builder *builder,
                                          accesskit_text_decoration value);

void accesskit_node_builder_clear_underline(struct accesskit_node_builder *builder);

struct accesskit_lengths accesskit_node_character_lengths(const struct accesskit_node *node);

struct accesskit_lengths accesskit_node_builder_character_lengths(const struct accesskit_node_builder *builder);

/**
 * The caller is responsible for freeing `value`.
 */
void accesskit_node_builder_set_character_lengths(struct accesskit_node_builder *builder,
                                                  size_t length,
                                                  const uint8_t *values);

void accesskit_node_builder_clear_character_lengths(struct accesskit_node_builder *builder);

struct accesskit_lengths accesskit_node_word_lengths(const struct accesskit_node *node);

struct accesskit_lengths accesskit_node_builder_word_lengths(const struct accesskit_node_builder *builder);

/**
 * The caller is responsible for freeing `value`.
 */
void accesskit_node_builder_set_word_lengths(struct accesskit_node_builder *builder,
                                             size_t length,
                                             const uint8_t *values);

void accesskit_node_builder_clear_word_lengths(struct accesskit_node_builder *builder);

struct accesskit_opt_coords accesskit_node_character_positions(const struct accesskit_node *node);

struct accesskit_opt_coords accesskit_node_builder_character_positions(const struct accesskit_node_builder *builder);

/**
 * The caller is responsible for freeing `value`.
 */
void accesskit_node_builder_set_character_positions(struct accesskit_node_builder *builder,
                                                    size_t length,
                                                    const float *values);

void accesskit_node_builder_clear_character_positions(struct accesskit_node_builder *builder);

struct accesskit_opt_coords accesskit_node_character_widths(const struct accesskit_node *node);

struct accesskit_opt_coords accesskit_node_builder_character_widths(const struct accesskit_node_builder *builder);

/**
 * The caller is responsible for freeing `value`.
 */
void accesskit_node_builder_set_character_widths(struct accesskit_node_builder *builder,
                                                 size_t length,
                                                 const float *values);

void accesskit_node_builder_clear_character_widths(struct accesskit_node_builder *builder);

struct accesskit_opt_bool accesskit_node_is_expanded(const struct accesskit_node *node);

struct accesskit_opt_bool accesskit_node_builder_is_expanded(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_expanded(struct accesskit_node_builder *builder, bool value);

void accesskit_node_builder_clear_expanded(struct accesskit_node_builder *builder);

struct accesskit_opt_bool accesskit_node_is_selected(const struct accesskit_node *node);

struct accesskit_opt_bool accesskit_node_builder_is_selected(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_selected(struct accesskit_node_builder *builder, bool value);

void accesskit_node_builder_clear_selected(struct accesskit_node_builder *builder);

struct accesskit_opt_name_from accesskit_node_name_from(const struct accesskit_node *node);

struct accesskit_opt_name_from accesskit_node_builder_name_from(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_name_from(struct accesskit_node_builder *builder,
                                          accesskit_name_from value);

void accesskit_node_builder_clear_name_from(struct accesskit_node_builder *builder);

struct accesskit_opt_description_from accesskit_node_description_from(const struct accesskit_node *node);

struct accesskit_opt_description_from accesskit_node_builder_description_from(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_description_from(struct accesskit_node_builder *builder,
                                                 accesskit_description_from value);

void accesskit_node_builder_clear_description_from(struct accesskit_node_builder *builder);

struct accesskit_opt_invalid accesskit_node_invalid(const struct accesskit_node *node);

struct accesskit_opt_invalid accesskit_node_builder_invalid(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_invalid(struct accesskit_node_builder *builder,
                                        accesskit_invalid value);

void accesskit_node_builder_clear_invalid(struct accesskit_node_builder *builder);

struct accesskit_opt_checked_state accesskit_node_checked_state(const struct accesskit_node *node);

struct accesskit_opt_checked_state accesskit_node_builder_checked_state(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_checked_state(struct accesskit_node_builder *builder,
                                              accesskit_checked_state value);

void accesskit_node_builder_clear_checked_state(struct accesskit_node_builder *builder);

struct accesskit_opt_live accesskit_node_live(const struct accesskit_node *node);

struct accesskit_opt_live accesskit_node_builder_live(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_live(struct accesskit_node_builder *builder, accesskit_live value);

void accesskit_node_builder_clear_live(struct accesskit_node_builder *builder);

struct accesskit_opt_default_action_verb accesskit_node_default_action_verb(const struct accesskit_node *node);

struct accesskit_opt_default_action_verb accesskit_node_builder_default_action_verb(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_default_action_verb(struct accesskit_node_builder *builder,
                                                    accesskit_default_action_verb value);

void accesskit_node_builder_clear_default_action_verb(struct accesskit_node_builder *builder);

struct accesskit_opt_text_direction accesskit_node_text_direction(const struct accesskit_node *node);

struct accesskit_opt_text_direction accesskit_node_builder_text_direction(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_text_direction(struct accesskit_node_builder *builder,
                                               accesskit_text_direction value);

void accesskit_node_builder_clear_text_direction(struct accesskit_node_builder *builder);

struct accesskit_opt_orientation accesskit_node_orientation(const struct accesskit_node *node);

struct accesskit_opt_orientation accesskit_node_builder_orientation(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_orientation(struct accesskit_node_builder *builder,
                                            accesskit_orientation value);

void accesskit_node_builder_clear_orientation(struct accesskit_node_builder *builder);

struct accesskit_opt_sort_direction accesskit_node_sort_direction(const struct accesskit_node *node);

struct accesskit_opt_sort_direction accesskit_node_builder_sort_direction(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_sort_direction(struct accesskit_node_builder *builder,
                                               accesskit_sort_direction value);

void accesskit_node_builder_clear_sort_direction(struct accesskit_node_builder *builder);

struct accesskit_opt_aria_current accesskit_node_aria_current(const struct accesskit_node *node);

struct accesskit_opt_aria_current accesskit_node_builder_aria_current(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_aria_current(struct accesskit_node_builder *builder,
                                             accesskit_aria_current value);

void accesskit_node_builder_clear_aria_current(struct accesskit_node_builder *builder);

struct accesskit_opt_has_popup accesskit_node_has_popup(const struct accesskit_node *node);

struct accesskit_opt_has_popup accesskit_node_builder_has_popup(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_has_popup(struct accesskit_node_builder *builder,
                                          accesskit_has_popup value);

void accesskit_node_builder_clear_has_popup(struct accesskit_node_builder *builder);

struct accesskit_opt_list_style accesskit_node_list_style(const struct accesskit_node *node);

struct accesskit_opt_list_style accesskit_node_builder_list_style(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_list_style(struct accesskit_node_builder *builder,
                                           accesskit_list_style value);

void accesskit_node_builder_clear_list_style(struct accesskit_node_builder *builder);

struct accesskit_opt_text_align accesskit_node_text_align(const struct accesskit_node *node);

struct accesskit_opt_text_align accesskit_node_builder_text_align(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_text_align(struct accesskit_node_builder *builder,
                                           accesskit_text_align value);

void accesskit_node_builder_clear_text_align(struct accesskit_node_builder *builder);

struct accesskit_opt_vertical_offset accesskit_node_vertical_offset(const struct accesskit_node *node);

struct accesskit_opt_vertical_offset accesskit_node_builder_vertical_offset(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_vertical_offset(struct accesskit_node_builder *builder,
                                                accesskit_vertical_offset value);

void accesskit_node_builder_clear_vertical_offset(struct accesskit_node_builder *builder);

const struct accesskit_affine *accesskit_node_transform(const struct accesskit_node *node);

const struct accesskit_affine *accesskit_node_builder_transform(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_transform(struct accesskit_node_builder *builder,
                                          struct accesskit_affine value);

void accesskit_node_builder_clear_transform(struct accesskit_node_builder *builder);

struct accesskit_opt_rect accesskit_node_bounds(const struct accesskit_node *node);

struct accesskit_opt_rect accesskit_node_builder_bounds(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_bounds(struct accesskit_node_builder *builder,
                                       struct accesskit_rect value);

void accesskit_node_builder_clear_bounds(struct accesskit_node_builder *builder);

struct accesskit_opt_text_selection accesskit_node_text_selection(const struct accesskit_node *node);

struct accesskit_opt_text_selection accesskit_node_builder_text_selection(const struct accesskit_node_builder *builder);

void accesskit_builder_set_text_selection(struct accesskit_node_builder *builder,
                                          struct accesskit_text_selection value);

void accesskit_node_builder_clear_text_selection(struct accesskit_node_builder *builder);

struct accesskit_custom_actions accesskit_node_custom_actions(const struct accesskit_node *node);

struct accesskit_custom_actions accesskit_node_builder_custom_actions(const struct accesskit_node_builder *builder);

void accesskit_node_builder_set_custom_actions(struct accesskit_node_builder *builder,
                                               size_t length,
                                               struct accesskit_custom_action *values);

void accesskit_node_builder_push_custom_action(struct accesskit_node_builder *builder,
                                               struct accesskit_custom_action item);

void accesskit_node_builder_clear_custom_actions(struct accesskit_node_builder *builder);

struct accesskit_opt_node_id accesskit_node_id_new(uint64_t id);

struct accesskit_node_builder *accesskit_node_builder_new(accesskit_role role);

/**
 * Converts an `accesskit_node_builder` to an `accesskit_node`, freeing the memory in the process.
 */
struct accesskit_node *accesskit_node_builder_build(struct accesskit_node_builder *builder,
                                                    struct accesskit_node_class_set *classes);

/**
 * Only call this function if you have to abort the building of a node.
 *
 * If you called `accesskit_node_builder_build`, don't call this function.
 */
void accesskit_node_builder_free(struct accesskit_node_builder *builder);

struct accesskit_tree accesskit_tree_new(struct accesskit_node_id root);

struct accesskit_action_handler *accesskit_action_handler_new(accesskit_action_handler_callback callback,
                                                              void *userdata);

void accesskit_action_handler_free(struct accesskit_action_handler *handler);

struct accesskit_affine accesskit_affine_identity(void);

struct accesskit_affine accesskit_affine_flip_y(void);

struct accesskit_affine accesskit_affine_flip_x(void);

struct accesskit_affine accesskit_affine_scale(double s);

struct accesskit_affine accesskit_affine_scale_non_uniform(double s_x, double s_y);

struct accesskit_affine accesskit_affine_rotate(double th);

struct accesskit_affine accesskit_affine_translate(struct accesskit_vec2 p);

struct accesskit_affine accesskit_affine_map_unit_square(struct accesskit_rect rect);

double accesskit_affine_determinant(struct accesskit_affine affine);

struct accesskit_affine accesskit_affine_inverse(struct accesskit_affine affine);

struct accesskit_rect accesskit_affine_transform_rect_bbox(struct accesskit_affine affine,
                                                           struct accesskit_rect rect);

bool accesskit_affine_is_finite(const struct accesskit_affine *affine);

bool accesskit_affine_is_nan(const struct accesskit_affine *affine);

struct accesskit_vec2 accesskit_point_to_vec2(struct accesskit_point point);

struct accesskit_rect accesskit_rect_from_points(struct accesskit_point p0,
                                                 struct accesskit_point p1);

struct accesskit_rect accesskit_rect_from_origin_size(struct accesskit_point origin,
                                                      struct accesskit_size size);

struct accesskit_rect accesskit_rect_with_origin(struct accesskit_rect rect,
                                                 struct accesskit_point origin);

struct accesskit_rect accesskit_rect_with_size(struct accesskit_rect rect,
                                               struct accesskit_size size);

double accesskit_rect_width(const struct accesskit_rect *rect);

double accesskit_rect_height(const struct accesskit_rect *rect);

double accesskit_rect_min_x(const struct accesskit_rect *rect);

double accesskit_rect_max_x(const struct accesskit_rect *rect);

double accesskit_rect_min_y(const struct accesskit_rect *rect);

double accesskit_rect_max_y(const struct accesskit_rect *rect);

struct accesskit_point accesskit_rect_origin(const struct accesskit_rect *rect);

struct accesskit_size accesskit_rect_size(const struct accesskit_rect *rect);

struct accesskit_rect accesskit_rect_abs(const struct accesskit_rect *rect);

double accesskit_rect_area(const struct accesskit_rect *rect);

bool accesskit_rect_is_empty(const struct accesskit_rect *rect);

bool accesskit_rect_contains(const struct accesskit_rect *rect, struct accesskit_point point);

struct accesskit_rect accesskit_rect_union(const struct accesskit_rect *rect,
                                           struct accesskit_rect other);

struct accesskit_rect accesskit_rect_union_pt(const struct accesskit_rect *rect,
                                              struct accesskit_point pt);

struct accesskit_rect accesskit_rect_intersect(const struct accesskit_rect *rect,
                                               struct accesskit_rect other);

struct accesskit_vec2 accesskit_size_to_vec2(struct accesskit_size size);

struct accesskit_point accesskit_vec2_to_point(struct accesskit_vec2 vec2);

struct accesskit_size accesskit_vec2_to_size(struct accesskit_vec2 vec2);

#if defined(__APPLE__)
/**
 * Memory is also freed when calling this function.
 */
void accesskit_macos_queued_events_raise(struct accesskit_macos_queued_events *events);
#endif

#if defined(__APPLE__)
struct accesskit_macos_adapter *accesskit_macos_adapter_new(void *view,
                                                            struct accesskit_tree_update initial_state,
                                                            struct accesskit_action_handler *handler);
#endif

#if defined(__APPLE__)
void accesskit_macos_adapter_free(struct accesskit_macos_adapter *adapter);
#endif

#if defined(__APPLE__)
struct accesskit_macos_queued_events *accesskit_macos_adapter_update(const struct accesskit_macos_adapter *adapter,
                                                                     struct accesskit_tree_update update);
#endif

#if defined(__APPLE__)
NSArray<NSObject> *accesskit_macos_adapter_view_children(const struct accesskit_macos_adapter *adapter);
#endif

#if defined(__APPLE__)
NSObject *accesskit_macos_adapter_focus(const struct accesskit_macos_adapter *adapter);
#endif

#if defined(__APPLE__)
NSObject *accesskit_macos_adapter_hit_test(const struct accesskit_macos_adapter *adapter,
                                           NSPoint point);
#endif

#if defined(__APPLE__)
struct accesskit_macos_subclassing_adapter *accesskit_macos_subclassing_adapter_new(void *view,
                                                                                    accesskit_tree_update_factory source,
                                                                                    void *source_userdata,
                                                                                    struct accesskit_action_handler *handler);
#endif

#if defined(__APPLE__)
void accesskit_macos_subclassing_adapter_free(struct accesskit_macos_subclassing_adapter *adapter);
#endif

#if defined(__APPLE__)
struct accesskit_macos_queued_events *accesskit_macos_subclassing_adapter_update(const struct accesskit_macos_subclassing_adapter *adapter,
                                                                                 struct accesskit_tree_update update);
#endif

#if defined(__APPLE__)
/**
 * This function will return a null pointer if there are no events to raise.
 */
struct accesskit_macos_queued_events *accesskit_macos_subclassing_adapter_update_if_active(const struct accesskit_macos_subclassing_adapter *adapter,
                                                                                           accesskit_tree_update_factory update_factory,
                                                                                           void *update_factory_userdata);
#endif

#if (defined(__linux__) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
/**
 * Caller is responsible for freeing `app_name`, `toolkit_name` and `toolkit_version`.
 */
struct accesskit_unix_adapter *accesskit_unix_adapter_new(const char *app_name,
                                                          const char *toolkit_name,
                                                          const char *toolkit_version,
                                                          struct accesskit_tree_update (*initial_state)(void*),
                                                          void *initial_state_userdata,
                                                          struct accesskit_action_handler *handler);
#endif

#if (defined(__linux__) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
void accesskit_unix_adapter_free(struct accesskit_unix_adapter *adapter);
#endif

#if (defined(__linux__) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
void accesskit_unix_adapter_set_root_window_bounds(const struct accesskit_unix_adapter *adapter,
                                                   struct accesskit_rect outer,
                                                   struct accesskit_rect inner);
#endif

#if (defined(__linux__) || defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__))
void accesskit_unix_adapter_update(const struct accesskit_unix_adapter *adapter,
                                   struct accesskit_tree_update update);
#endif

#if defined(_WIN32)
struct accesskit_windows_uia_init_marker *accesskit_windows_uia_init_marker_new(void);
#endif

#if defined(_WIN32)
/**
 * You don't need to call this if you use `accesskit_windows_adapter_new`.
 */
void accesskit_windows_uia_init_marker_free(struct accesskit_windows_uia_init_marker *marker);
#endif

#if defined(_WIN32)
/**
 * Memory is also freed when calling this function.
 */
void accesskit_windows_queued_events_raise(struct accesskit_windows_queued_events *events);
#endif

#if defined(_WIN32)
struct accesskit_windows_adapter *accesskit_windows_adapter_new(HWND hwnd,
                                                                struct accesskit_tree_update initial_state,
                                                                struct accesskit_action_handler *handler,
                                                                struct accesskit_windows_uia_init_marker *uia_init_marker);
#endif

#if defined(_WIN32)
void accesskit_windows_adapter_free(struct accesskit_windows_adapter *adapter);
#endif

#if defined(_WIN32)
struct accesskit_windows_queued_events *accesskit_windows_adapter_update(const struct accesskit_windows_adapter *adapter,
                                                                         struct accesskit_tree_update update);
#endif

#if defined(_WIN32)
struct accesskit_opt_lresult accesskit_windows_adapter_handle_wm_getobject(struct accesskit_windows_adapter *adapter,
                                                                           WPARAM wparam,
                                                                           LPARAM lparam);
#endif

#if defined(_WIN32)
struct accesskit_windows_subclassing_adapter *accesskit_windows_subclassing_adapter_new(HWND hwnd,
                                                                                        accesskit_tree_update_factory source,
                                                                                        void *source_userdata,
                                                                                        struct accesskit_action_handler *handler);
#endif

#if defined(_WIN32)
void accesskit_windows_subclassing_adapter_free(struct accesskit_windows_subclassing_adapter *adapter);
#endif

#if defined(_WIN32)
struct accesskit_windows_queued_events *accesskit_windows_subclassing_adapter_update(const struct accesskit_windows_subclassing_adapter *adapter,
                                                                                     struct accesskit_tree_update update);
#endif

#if defined(_WIN32)
/**
 * This function will return a null pointer if there are no events to raise.
 */
struct accesskit_windows_queued_events *accesskit_windows_subclassing_adapter_update_if_active(const struct accesskit_windows_subclassing_adapter *adapter,
                                                                                               accesskit_tree_update_factory update_factory,
                                                                                               void *update_factory_userdata);
#endif

#endif /* ACCESSKIT_H */
