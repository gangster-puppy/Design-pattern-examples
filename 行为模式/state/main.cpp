#include<iostream>
using namespace std;
class Context;
class State {
protected:
    Context *m_context;
public:
    virtual ~State(){} // 状态基类虚析构
    void setContext(Context *ctx){ this->m_context = ctx; }
    virtual void handler1() = 0; // 纯虚函数，子类继承时必须要加override去重写
    virtual void handler2() = 0;
};

class Context {
private:
    State *m_state; // 这个状态指针的作用是什么？
public:
    Context(State *state) : m_state(state) { this->transitionTo(state); }
    ~Context() { delete m_state; }
    void transitionTo(State *state) { 
        cout << "Context transition to " << typeid(*state).name() << endl;
        if (this->m_state != nullptr)
            delete this->m_state; // Context中管理State的生命周期
        this->m_state = state;
        this->m_state->setContext(this);
    }
    void Request1() { this->m_state->handler1(); }
    void Request2() { this->m_state->handler2(); }
};

class ConcreateStateA : public State {
    void handler1() override { cout << "ConcreateStateA handlers request1" << endl; }
    void handler2() override {};
};

class ConcreateStateB : public State {
    void handler1() override { cout << "ConcreateStateB handlers request1" << endl; }
    void handler2() override { 
        cout << "ConcreateStateB handlers request2" << endl; 
        this->m_context->transitionTo(new ConcreateStateA);
    }
};

int main() {
    Context *context = new Context(new ConcreateStateA);
    context->Request1();
    context->Request2();
    delete context;
    return 0;
}
