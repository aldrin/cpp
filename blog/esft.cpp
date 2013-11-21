// Copyright (C) 2013, Aldrin D'Souza.
// License: http://opensource.org/licenses/BSD-2-Clause

#include <memory>
#include <vector>
#include <cassert>

using namespace std;

// forward declaration
struct student;

// models a course being offered
struct course
{
  // enroll a student for this course
  void enroll(const shared_ptr<student>& s){ enrolled_.push_back(s); }
private:
  // the list of enrolled students.
  vector<shared_ptr<student >> enrolled_;
};

// models a student
struct student
{
  // offer a course and the student enrolls if interested
  void offer(course& c) { if(interested_in(c)) { c.enroll(shared_from_this()); } }
  // how do we implement this?
  virtual shared_ptr<student> shared_from_this() = 0;
private:
  // internal implementation of what interests a student
  bool interested_in(course& c) { return true; }
};

void test_wp_expected_usage()
{
  shared_ptr<int> p(new int); // a shared pointer
  weak_ptr<int> w(p); // make a weak reference
  {
    assert(1 == p.use_count()); // only 1 owner: p
    shared_ptr<int> q = w.lock(); // encash the weak ref
    assert(2 == q.use_count()); // 2 owners now: p, q
  } // q goes out of scope
  assert(1 == w.use_count()); // we're back to one owner
  p.reset(); // explicitly reset p.
  assert(w.expired()); // w knows it is dangling now.
}

void test_sp_expected_usage()
{
  int *i = new int; // the raw pointer
  shared_ptr<int> p(i); // wrap it up
  assert(p.unique()); // p is the sole owner
  {
    shared_ptr<int> q(p); // share owner ship
    assert(2==q.use_count()); // 2 onwers for the pointer
    assert(!(q.unique() || p.unique())); // both p & q know they're not unique.
    assert(p.use_count()==q.use_count()); // use counts agree
  }
  assert(1==p.use_count()); // q out of scope
  assert(p.unique()); // p is the sole owner again.
}

void test_sp_bad_usage()
{
  int *i = new int; // the raw pointer
  shared_ptr<int> p(i); // wrap it up
  shared_ptr<int> q(i); // wrap it again (bad)
} // <- invalid delete (double delete)

void test_sp_bad_usage2()
{
  int i; // on the stack
  shared_ptr<int> p(&i); // wrap it up
} // <- invalid delete (bad free)

struct obviously_bad_student: public student
{
  shared_ptr<student> shared_from_this(){ return shared_ptr<student>(this); }
};

struct self_aware_leaky_student: public student
{
  // let an instance know its wrapped in a shared pointer
  void know_thyself(shared_ptr<student> s) { self_ = s; }
  // return the stored pointer
  shared_ptr<student> shared_from_this() { return self_; }
private:
  shared_ptr<student> self_;
};

struct almost_right_student: public student
{
  // let an instance know its wrapped in a shared pointer
  void know_thyself(shared_ptr<student> s) { self_ = s; }
  shared_ptr<student> shared_from_this() { return self_.lock(); }
private:
  weak_ptr<student> self_;
};

int main()
{
  course math;
  shared_ptr<almost_right_student> s = make_shared<almost_right_student>();
  s->know_thyself(s);
  s->offer(math);
  assert(s.use_count()==2);   // enrolled_, s
}

/*
   // The solution
   struct student : public enable_shared_from_this<student>
   {
    // offer a course and the student enrolls if interested
    void offer(course& c) { if(interested_in(c)) { c.enroll(shared_from_this());
       } }
   private:
    // internal implementation of what interests a student
    bool interested_in(course& c) { return true; }
   };
 */
