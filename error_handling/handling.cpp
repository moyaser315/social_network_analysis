#include "./handling.h"

/// @brief detects errors and correct missing and closing tags
/// @param xml the string to parse for error detection
void detect(string &xml)
{
    string op, close;
    bool f = 0;
    for (int i = 0; i < xml.size(); i++)
    {
        if (xml[i] == '<')
        {
            // if f is raised it's a closing after a value
            if (f)
            {
                if (xml[i + 1] != '/')
                {
                    cout << "missing closing tag for : " << tags.top() << "\n";
                    cout << "will be fixed immeditly for file consictincy\n";
                    xml = xml.substr(0, i) + "</" + tags.top() + '>' + xml.substr(i);

                    i += tags.top().size() + 2;
                    tags.pop();
                }
                f = 0;
            }
            // see if it's closing
            if (xml[i + 1] == '/')
            {
                i++;
                int st = i;
                while (xml[++i] != '>')
                {
                    close += xml[i];
                }
                i--;
                // if the stack is empty there are no opening tags and that means there was a missing opening tag
                if (tags.empty())
                {
                    cout << "missing opening tag for : " << close << "\n";
                    cout << "will be fixed immeditly for file consictincy\n";
                    xml = xml.substr(0, st - 1) + '<' + close + '>' + xml.substr(st - 1);
                }

                else
                {
                    // if closing == opening dismiss
                    if (tags.top() == close)
                        tags.pop();

                    else
                    {
                        // see if closing or opening is missing or is it mismatch

                        cout << "closing tag mismatch for : " << tags.top() << "\n";
                        cout << close << "   " << tags.top() << "\n";
                        err.push(tags.top());
                        tags.pop();
                        cout << close << "   " << tags.top() << "\n";
                        positions.push_back(st);
                    }
                }
            }
            else
            {
                // get the opening to push to the stack
                while (xml[++i] != '>')
                {
                    op += xml[i];
                }
                tags.push(op);

                op = "";
                i--;
            }
            f = 0;
            close = "";
        }
        // if it's text ignore but raise a flag that it needs closing
        else if (xml[i] == '>')
        {
            if (xml[i + 1] == '<')
            {
                f = 0;
            }
            else
            {
                f = 1;
            }
        }
        else
            continue;
    }
    bool e = 0;
}
