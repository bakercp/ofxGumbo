// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"
#include <assert.h>


static std::string find_line(const std::string& original_text, const GumboAttribute& attr)
{
    size_t attr_index = attr.original_value.data - original_text.data();
    size_t begin = original_text.rfind("\n", attr_index) + 1;
    size_t end = original_text.find("\n", attr_index);
    if (end != std::string::npos) {
        end--;
    } else {
        end = (size_t) original_text.length() - 1;
    }
    end = std::min(end, attr_index + 40);
    begin = std::max(begin, attr_index - 40);
    return original_text.substr(begin, end - begin);
}

static void search_for_class(GumboNode* node,
                             const std::string& original_text,
                             const std::string& cls_name)
{
    if (node->type != GUMBO_NODE_ELEMENT)
    {
        return;
    }

    GumboAttribute* cls_attr;

    if ((cls_attr = gumbo_get_attribute(&node->v.element.attributes, "class")) &&
        strstr(cls_attr->value, cls_name.c_str()) != NULL) {
        std::cout << cls_attr->value_start.line << ":"
        << cls_attr->value_start.column << " - "
        << find_line(original_text, *cls_attr) << std::endl;
    }

    GumboVector* children = &node->v.element.children;

    for (int i = 0; i < children->length; ++i)
    {
        search_for_class(static_cast<GumboNode*>(children->data[i]), original_text, cls_name);
    }
}

void ofApp::setup()
{
    ofBuffer buffer = ofBufferFromFile("gumbo-parser.html");

    GumboOutput* output = gumbo_parse(buffer.getText().c_str());

    std::string classToFind = "anchor";

    search_for_class(output->root, buffer.getText(), classToFind);

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}


void ofApp::draw()
{
    ofBackground(0);
    ofDrawBitmapString("See the console.", ofVec2f(8, 14));
}
