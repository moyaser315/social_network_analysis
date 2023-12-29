#include "./handling.h"

using namespace std;

stack<string> tags;
stack<string> err;
vector<int> positions;

/// @brief detects errors and correct missing and closing tags
/// @param xml the string to parse for error detection
void detect(string &xml) {
    string op, close;
    bool f = 0;
    for (int i = 0; i < xml.size(); i++) {
        if (xml[i] == '<') {
            // if f is raised it's a closing after a value
            if (f) {
                if (xml[i + 1] != '/') {
                    int temp = get_freq(tags.top());
                    if (freq[temp] > 0)
                        freq[temp]--;
                    cout << "missing closing tag for : " << tags.top() << "\n";
                    cout << "will be fixed immeditly for file consictincy\n";
                    xml = xml.substr(0, i) + "</" + tags.top() + '>' +
                          xml.substr(i);

                    i += tags.top().size() + 3;
                    tags.pop();
                }
                f = 0;
            }
            // see if it's closing
            if (xml[i + 1] == '/') {
                i++;
                int st = i;
                while (xml[++i] != '>') {
                    close += xml[i];
                }
                i--;
                // if the stack is empty there are no opening tags and that
                // means there was a missing opening tag
                if (tags.empty()) {
                    cout << "missing opening tag for : " << close << "\n";
                    cout << "will be fixed immeditly for file consictincy\n";
                    xml = xml.substr(0, st - 1) + '<' + close + '>' +
                          xml.substr(st - 1);
                }

                else {
                    // if closing == opening dismiss
                    if (tags.top() == close)
                        tags.pop();

                    else {
                        // see if closing or opening is missing or is it
                        // mismatch
                        int temp = get_freq(close);
                        if (freq[temp] > 0) {
                            cout << "entered fre < 0 missing closing tag for : "
                                 << tags.top() << "\n";
                            cout << "will be fixed immeditly for file "
                                    "consictincy\n";
                            xml = xml.substr(0, i) + "</" + tags.top() + '>' +
                                  xml.substr(i);
                            cout << "\n\n" << xml.substr(st - 5, 15) << "\n\n";
                            freq[temp]--;
                            tags.pop();
                        } else if (freq[temp] < 0) {
                            cout << " missing opening tag for : " << close
                                 << "\n";
                            cout << "will be fixed immeditly for file "
                                    "consictincy\n";
                            xml = xml.substr(0, st - 1) + '<' + close + '>' +
                                  xml.substr(st - 1);
                            cout << "\n\n" << xml.substr(st - 20, 40) << "\n\n";
                            tags.push(close);
                            freq[temp]++;
                        } else {
                            cout << freq[temp] << "  ";
                            cout << "closing tag mismatch for : " << tags.top()
                                 << "\n";
                            cout << close << "   " << tags.top() << "\n";
                            err.push(tags.top());
                            tags.pop();
                            cout << close << "   " << tags.top() << "\n";
                            positions.push_back(st);
                        }
                    }
                }
            } else {
                // get the opening to push to the stack
                while (xml[++i] != '>') {
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
        else if (xml[i] == '>') {
            if (xml[i + 1] == '<') {
                f = 0;
            } else {
                f = 1;
            }
        } else
            continue;
    }
    bool e = 0;
    // output if there are still missing tags we couldn't handle and give a
    // warning that may have caused mismatch
    for (int i = 0; i < 11; i++) {
        if (freq[i] < 0) {
            e = 1;
            cout << "there was a missing opening tag for : " << IDs[i] << "\n";
        }
        if (freq[i] > 0) {
            e = 1;
            cout << "there was a missing closing tag for : " << IDs[i] << "\n";
        }
    }
    if (e)
        cout << "IMPORTANT NOTE !!!\nmissing tags may have caused mismatch "
                "errors\n";
}

/// @brief gets the position of the string in the frequency array
/// @param s the tag we need to know its frequency
/// @return the index to know the frequency
int get_freq(string s) {
    for (int i = 0; i < 11; i++) {
        if (s == IDs[i])
            return i;
    }
}
/// @brief corrects mismatched strings
/// @param xml the cml to correct
void correct(string &xml) {
    string s;
    int pos;
    if (err.empty()) {
        cout << "all correct !\n";
        return;
    }
    while (!err.empty()) {
        s = err.top();
        pos = positions.back();
        err.pop();
        positions.pop_back();
        cout << "************\n";
        cout << "case 2 mismatch\n";
        for (int i = pos - 10; i < xml.size() && i < pos + 10; i++)
            cout << xml[i];
        cout << endl << s;
        cout << "\n************\n";
        int en = pos;
        while (xml[en] != '>') {
            en++;
        }
        xml = xml.substr(0, pos + 1) + s + xml.substr(en);
        cout << "************\n";
        for (int i = pos - 10; i < xml.size() && i < pos + 10; i++)
            cout << xml[i];
        cout << endl << s;
        cout << "\n************\n\n\n";
    }
}

