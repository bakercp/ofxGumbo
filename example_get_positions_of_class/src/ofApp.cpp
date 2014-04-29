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


static std::string find_title(const GumboNode* root)
{
    assert(root->type == GUMBO_NODE_ELEMENT);
    assert(root->v.element.children.length >= 2);

    const GumboVector* root_children = &root->v.element.children;

    GumboNode* head = 0;

    for (int i = 0; i < root_children->length; ++i)
    {
        GumboNode* child = (GumboNode*)root_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
            child->v.element.tag == GUMBO_TAG_HEAD)
        {
            head = child;
            break;
        }
    }

    assert(0 != head);

    GumboVector* head_children = &head->v.element.children;

    for (int i = 0; i < head_children->length; ++i)
    {
        GumboNode* child = (GumboNode*)head_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
            child->v.element.tag == GUMBO_TAG_TITLE)
        {
            if (1 != child->v.element.children.length)
            {
                return "<empty title>";
            }

            GumboNode* title_text = (GumboNode*)child->v.element.children.data[0];

            assert(title_text->type == GUMBO_NODE_TEXT);

            return 0 != title_text->v.text.text ? title_text->v.text.text : "<error>";
        }
    }

    return "<no title found>";
}

void ofApp::setup()
{
    ofBuffer buffer = ofBufferFromFile("gumbo-parser.html");

    GumboOutput* output = gumbo_parse(buffer.getText().c_str());

    std::cout << find_title(output->root) << std::endl;

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}


void ofApp::draw()
{
    ofBackground(0);
    ofDrawBitmapString("See the console.", ofVec2f(8, 14));
}
