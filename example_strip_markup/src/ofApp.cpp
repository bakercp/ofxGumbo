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
#include "assert.h"


static std::string cleantext(GumboNode* node)
{
    if (node->type == GUMBO_NODE_TEXT)
    {
        return std::string(node->v.text.text);
    }
    else if (node->type == GUMBO_NODE_ELEMENT
          && node->v.element.tag != GUMBO_TAG_SCRIPT
          && node->v.element.tag != GUMBO_TAG_STYLE)
    {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (int i = 0; i < children->length; ++i)
        {
            const std::string text = cleantext((GumboNode*) children->data[i]);

            if (i != 0 && !text.empty())
            {
                contents.append(" ");
            }

            contents.append(text);
        }

        return contents;
    }
    else
    {
        return "";
    }
}


void ofApp::setup()
{
    ofBuffer buffer = ofBufferFromFile("gumbo-parser.html");

    GumboOutput* output = gumbo_parse(buffer.getText().c_str());

    std::cout << cleantext(output->root) << std::endl;

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}


void ofApp::draw()
{
    ofBackground(0);
    ofDrawBitmapString("See the console.", ofVec2f(8, 14));
}
