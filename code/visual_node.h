#pragma once

#include "settings.h"
#include "vmath.h"
#include <iostream>
#include <pugixml.hpp>
#include <raylib.h>
#include <string>
#include <yoga/Yoga.h>

class VisualNode {
private:
  YGNodeRef node;
  std::string name;
  void (*render_func)(VisualNode* visual);

public:
  Color color;

  std::string text;
  int fontSize;

  VisualNode() {
    node = YGNodeNew();
    YGNodeSetContext(node, this);

    color.r = color.g = color.b = color.a = 0;
    name = "";
    text = "";
    fontSize = 0;
    render_func = nullptr;
  }

  float GetTop() { return YGNodeLayoutGetTop(node); }
  float GetLeft() { return YGNodeLayoutGetLeft(node); }
  float GetWidth() { return YGNodeLayoutGetWidth(node); }
  float GetHeight() { return YGNodeLayoutGetHeight(node); }

  size_t GetChildCount() { return YGNodeGetChildCount(node); }
  VisualNode* GetChild(const size_t index) {
    auto child = YGNodeGetChild(node, index);
    return (VisualNode*)YGNodeGetContext(child);
  }

  void InsertChild(VisualNode* child, size_t index) { YGNodeInsertChild(node, child->node, index); }
  void AddChild(VisualNode* child) { YGNodeInsertChild(node, child->node, GetChildCount()); }
  void RemoveChild(VisualNode* child) { YGNodeRemoveChild(node, child->node); }

  VisualNode* GetOwner() {
    auto owner = YGNodeGetOwner(node);
    return (VisualNode*)YGNodeGetContext(owner);
  }

  void RemoveAllChildren() {
    size_t skipped = 0;
    while (GetChildCount() > skipped) {
      const auto child = GetChild(skipped);
      auto weOwn = child->GetOwner() == this;
      if (weOwn) {
        RemoveChild(child);

        delete child;
      } else {
        skipped++;
      }
    }
  }

  ~VisualNode() {
    RemoveAllChildren();
    YGNodeFree(node);
  }

  void Render() {
    if (render_func != nullptr)
      render_func(this);

    for (size_t i = 0; i < GetChildCount(); i++) {
      auto child = GetChild(i);
      child->Render();
    }
  }

  void RenderTree(float width, float height) {
    YGNodeCalculateLayout(node, width, height, YGDirectionLTR);
    Render();
  }

  VisualNode* FindByName(const std::string& n) {
    if (name == n)
      return this;

    for (size_t i = 0; i < GetChildCount(); i++) {
      auto subFind = GetChild(i)->FindByName(n);
      if (subFind != nullptr)
        return subFind;
    }

    return nullptr;
  }

  static void RenderNodeType(VisualNode* visual) {
    auto top = visual->GetTop();
    auto left = visual->GetLeft();
    auto width = visual->GetWidth();
    auto height = visual->GetHeight();

    DrawRectangle(left, top, width, height, visual->color);
  }

  static void RenderTextType(VisualNode* visual) {
    auto top = visual->GetTop();
    auto left = visual->GetLeft();
    auto text = visual->text.c_str();
    auto fontSize = visual->fontSize;

    DrawTextEx(font, text, Vector2{left, top}, fontSize, 0, visual->color);
  }

  static VisualNode* ParseNode(pugi::xml_node xml_child) {
    auto child = new VisualNode();

    for (auto attribute : xml_child.attributes()) {
      std::string name = attribute.name();
      if (name == "width") {
        std::string value = attribute.as_string();
        if (value.back() == '%') {
          YGNodeStyleSetWidthPercent(child->node, std::stoi(value.substr(0, value.size() - 1)));
        } else if (value == "auto") {
          YGNodeStyleSetWidthAuto(child->node);
        } else {
          YGNodeStyleSetWidth(child->node, attribute.as_float());
        }
      } else if (name == "height") {
        std::string value = attribute.as_string();
        if (value.back() == '%') {
          YGNodeStyleSetHeightPercent(child->node, std::stoi(value.substr(0, value.size() - 1)));
        } else if (value == "auto") {
          YGNodeStyleSetHeightAuto(child->node);
        } else {
          YGNodeStyleSetHeight(child->node, attribute.as_float());
        }
      } else if (name == "padding") {
        YGNodeStyleSetPadding(child->node, YGEdgeAll, attribute.as_float());
      } else if (name == "margin") {
        YGNodeStyleSetMargin(child->node, YGEdgeAll, attribute.as_float());
      } else if (name == "flex-grow") {
        YGNodeStyleSetFlexGrow(child->node, attribute.as_float());
      } else if (name == "gap") {
        YGNodeStyleSetGap(child->node, YGGutterAll, attribute.as_float());
      } else if (name == "color") {
        child->color = HexToRgb(attribute.as_string());
      } else if (name == "align-content") {
        std::string align = attribute.as_string();
        YGAlign value;
        if (align == "auto") {
          value = YGAlignAuto;
        } else if (align == "flex-start") {
          value = YGAlignFlexStart;
        } else if (align == "flex-end") {
          value = YGAlignFlexEnd;
        } else if (align == "center") {
          value = YGAlignCenter;
        } else if (align == "stretch") {
          value = YGAlignStretch;
        } else if (align == "baseline") {
          value = YGAlignBaseline;
        } else if (align == "space-between") {
          value = YGAlignSpaceBetween;
        } else if (align == "space-around") {
          value = YGAlignSpaceAround;
        } else if (align == "space-evenly") {
          value = YGAlignSpaceEvenly;
        }

        YGNodeStyleSetAlignContent(child->node, value);
      } else if (name == "align-items") {
        std::string align = attribute.as_string();
        YGAlign value;
        if (align == "auto") {
          value = YGAlignAuto;
        } else if (align == "flex-start") {
          value = YGAlignFlexStart;
        } else if (align == "flex-end") {
          value = YGAlignFlexEnd;
        } else if (align == "center") {
          value = YGAlignCenter;
        } else if (align == "stretch") {
          value = YGAlignStretch;
        } else if (align == "baseline") {
          value = YGAlignBaseline;
        } else if (align == "space-between") {
          value = YGAlignSpaceBetween;
        } else if (align == "space-around") {
          value = YGAlignSpaceAround;
        } else if (align == "space-evenly") {
          value = YGAlignSpaceEvenly;
        }

        YGNodeStyleSetAlignItems(child->node, value);
      } else if (name == "align-self") {
        std::string align = attribute.as_string();
        YGAlign value;
        if (align == "auto") {
          value = YGAlignAuto;
        } else if (align == "flex-start") {
          value = YGAlignFlexStart;
        } else if (align == "flex-end") {
          value = YGAlignFlexEnd;
        } else if (align == "center") {
          value = YGAlignCenter;
        } else if (align == "stretch") {
          value = YGAlignStretch;
        } else if (align == "baseline") {
          value = YGAlignBaseline;
        } else if (align == "space-between") {
          value = YGAlignSpaceBetween;
        } else if (align == "space-around") {
          value = YGAlignSpaceAround;
        } else if (align == "space-evenly") {
          value = YGAlignSpaceEvenly;
        }

        YGNodeStyleSetAlignSelf(child->node, value);
      } else if (name == "flex-wrap") {
        std::string wrap = attribute.as_string();
        YGWrap value;
        if (wrap == "nowrap") {
          value = YGWrapNoWrap;
        } else if (wrap == "wrap") {
          value = YGWrapWrap;
        } else if (wrap == "wrap-reverse") {
          value = YGWrapWrapReverse;
        }

        YGNodeStyleSetFlexWrap(child->node, value);
      } else if (name == "flex-direction") {
        std::string dir = attribute.as_string();
        YGFlexDirection value;

        if (dir == "column") {
          value = YGFlexDirectionColumn;
        } else if (dir == "column-reverse") {
          value = YGFlexDirectionColumnReverse;
        } else if (dir == "row") {
          value = YGFlexDirectionRow;
        } else if (dir == "row-reverse") {
          value = YGFlexDirectionRowReverse;
        }

        YGNodeStyleSetFlexDirection(child->node, value);
      } else if (name == "name") {
        child->name = attribute.as_string();
      }
    }

    return child;
  }

  static VisualNode* ParseText(pugi::xml_node xml_child) {
    auto child = ParseNode(xml_child);
    for (auto attribute : xml_child.attributes()) {
      std::string name = attribute.name();
      if (name == "font-size") {
        child->fontSize = attribute.as_int();
      }
    }

    child->text = Trim(xml_child.text().as_string());

    return child;
  }

  static void InsertTree(VisualNode* root, pugi::xml_node xml_child) {
    std::string type = xml_child.name();
    if (type == "Node") {
      auto child = ParseNode(xml_child);
      child->render_func = RenderNodeType;

      root->AddChild(child);

      for (auto subChild : xml_child.children()) {
        InsertTree(child, subChild);
      }
    } else if (type == "Text") {
      auto child = ParseText(xml_child);
      child->render_func = RenderTextType;

      root->AddChild(child);
    }
  }

  static VisualNode* FromXml(const pugi::xml_document& doc) {
    VisualNode* root;

    pugi::xml_node xml_child = *doc.children().begin();
    std::string type = xml_child.name();
    if (type == "Node") {
      root = ParseNode(xml_child);
      root->render_func = RenderNodeType;

      for (auto subChild : xml_child.children()) {
        InsertTree(root, subChild);
      }
    } else if (type == "Text") {
      root = ParseText(xml_child);
      root->render_func = RenderTextType;
    }

    return root;
  }

  static VisualNode* FromFile(std::string path) {
    pugi::xml_document document;

    if (!document.load_file(path.c_str()))
      return nullptr;

    return FromXml(document);
  }
};
