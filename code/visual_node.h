#include "raylib.h"
#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <yoga/Yoga.h>

class VisualNode {
private:
  YGNodeRef node;
  Color color;

public:
  VisualNode() {
    node = YGNodeNew();
    YGNodeSetContext(node, this);

    color.r = color.g = color.b = color.a = 255;
  }

  ~VisualNode() {
    YGNodeFreeRecursive(node); // TODO context delete
  }

  void RenderTreeFrom(YGNodeRef root, int deep = 0) {
    if (deep == 0)
      YGNodeCalculateLayout(root, GetRenderWidth(), GetRenderHeight(), YGDirectionLTR);

    auto top = YGNodeLayoutGetTop(root);
    auto left = YGNodeLayoutGetLeft(root);
    auto width = YGNodeLayoutGetWidth(root);
    auto height = YGNodeLayoutGetHeight(root);

    auto visual = (VisualNode*)YGNodeGetContext(root);
    DrawRectangle(left, top, width, height, visual->color);

    for (auto i = 0; i < YGNodeGetChildCount(root); i++) {
      auto child = YGNodeGetChild(root, i);
      RenderTreeFrom(child, deep + 1);
    }
  }

  void RenderTree() { RenderTreeFrom(node); }

  static void InsertTree(YGNodeRef root, pugi::xml_node xml_child) {
    auto child = new VisualNode();

    YGNodeInsertChild(root, child->node, YGNodeGetChildCount(root));

    for (auto attribute : xml_child.attributes()) {
      std::string name = attribute.name();
      if (name == "width") {
        YGNodeStyleSetWidth(child->node, attribute.as_float());
      } else if (name == "height") {
        YGNodeStyleSetHeight(child->node, attribute.as_float());
      } else if (name == "padding") {
        YGNodeStyleSetPadding(child->node, YGEdgeAll, attribute.as_float());
      } else if (name == "margin") {
        YGNodeStyleSetMargin(child->node, YGEdgeAll, attribute.as_float());
      } else if (name == "flex-grow") {
        YGNodeStyleSetFlexGrow(child->node, attribute.as_float());
      } else if (name == "gap") {
        YGNodeStyleSetGap(child->node, YGGutterAll, attribute.as_float());
      } else if (name == "color") {
        std::string str = attribute.as_string();

        child->color.r = std::stoi(str.substr(1, 2), 0, 16);
        child->color.g = std::stoi(str.substr(3, 2), 0, 16);
        child->color.b = std::stoi(str.substr(5, 2), 0, 16);

        if (str.length() == 9) {
          child->color.a = std::stoi(str.substr(7, 2), 0, 16);
        } else {
          child->color.a = 255;
        }
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
      }
    }

    for (auto subChild : xml_child.children()) {
      InsertTree(child->node, subChild);
    }
  }

  static VisualNode* FromXml(const pugi::xml_document& doc) {
    auto root = new VisualNode();
    for (auto child : doc) {
      InsertTree(root->node, child);
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