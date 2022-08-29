#ifndef MESSAGEQUEU_H
#define MESSAGEQUEU_H

#include <mutex>
#include <deque>
#include <condition_variable>


template <typename T>
class MessageQueue
{
public:
    
    MessageQueue(){};
    ~MessageQueue(){
        // std::cout << "MessageQueue desctructor called \n";
    };

    void send( T&& );
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _msgQuConVar;
    std::mutex _mqMutex;

    

    
};



/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    // to wait for and receive new messages and pull them from the queue using move semantics.
    // The received object should then be returned by the receive function.
    std::unique_lock lck(_mqMutex);
    T msg;
    _msgQuConVar.wait(lck, [this, &msg]() mutable
                      {
                          if (_queue.empty())
                              return false;
                          msg = std::move(_queue.front());
                          _queue.pop_front();
                          return true;
                      });

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // The method send should use the mechanisms std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    std::lock_guard(this->_mqMutex);
    _queue.clear();
    _queue.emplace_back(std::move(msg));

    _msgQuConVar.notify_one();
}

#endif