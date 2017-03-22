#include<iostream>
#include <memory>
#include"stack.h"


using namespace std;

int main()
{
    TwoSideStack<int> myStack1(10);

    int a = 1313;

    cout << "mystack1: " << endl;

    myStack1.printStack();

    int ct = 0;

    while (ct++ != 9){
        myStack1.push_top(ct);
        myStack1.push_bottom(ct);
    }

    TwoSideStack<int> myStack2(myStack1);

    cout << "mystack1: " << endl;
    myStack1.printStack();

    cout << "mystack2: " << endl;
    myStack2.printStack();

    for(int i = 0; i < 5; ++i)
    {
        myStack1.pop_bottom();
    }

    cout << "mystack1: " << endl;
    myStack1.printStack();
    cout << "mystack2: " << endl;
    myStack2.printStack();


    TwoSideStack<int>::const_iterator it = myStack1.bottom_const();

    for(TwoSideStack<int>::const_iterator i = myStack1.top_const(); i <= it; ++i)
        cout << *i << " : " << endl;

    cout << endl;

    for(TwoSideStack<int>::const_iterator i = myStack1.bottom_const(); i >= myStack1.top_const(); --i)
        cout << *i << " : " << endl;

    TwoSideStack<char> s1;
    for(char c = 'a'; c < 'f'; ++c)
        s1.push_top(c);

    TwoSideStack<char> s2(s1);
    TwoSideStack<char> s3(s2);

    cout << "1) refs == 3 \n" << s1 << s2 << s3;

    TwoSideStack<char> s4(s1);

    cout << s4;

    cout << "2) refs == 4 \n" << s1 << s2 << s3 << s4;
    s1.push_top('f');

    TwoSideStack<char> s5(s1);

    cout << "3) s1 == 2, s2-4 == 3 5 == 2\n" << s1 << s2 << s3 << s4 << s5;
    s5.push_top('z');

    cout << "4) s1 == 1, s2-4 == 3 5 == 1\n" << s1 << s2 << s3 << s4 << s5;

    s5.push_top('.');
    cout << "5)\n s1 == 1, s2-4 == 3 5 == 1" << s1 << s2 << s3 << s4 << s5;

    return 0;
}
