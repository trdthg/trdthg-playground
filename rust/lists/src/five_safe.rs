// An Unsafe Queue

/*
input list:
[Some(ptr)] -> (A, Some(ptr)) -> (B, None)

flipped push X:
[Some(ptr)] -> (A, Some(ptr)) -> (B, Some(ptr)) -> (X, None)

*/

// Layout
pub struct List<'a, T> {
    head: Link<T>,
    tail: Option<&'a mut Node<T>>,
}

type Link<T> = Option<Box<Node<T>>>;

struct Node<T> {
    elem: T,
    next: Link<T>,
}

// Basic
impl<'a, T> List<'a, T> {
    pub fn new() -> List<'a, T> {
        List { head: None, tail: None }
    }

    pub fn push(&'a mut self, elem: T) {
        let new_tail = Box::new(Node {
            elem: elem,
            // When you push onto the tail, your next is always None
            next: None,
        });

        // Put the box in the right place, and then grab a reference to its Node
        match self.tail.take() {
            Some(old_tail) => {
                // If the old tail existed, update it to point to the new tail
                old_tail.next = Some(new_tail);
                self.tail = old_tail.next.as_deref_mut()
            }
            None => {
                // Otherwise, update the head to point to it
                self.head = Some(new_tail);
                self.tail = self.head.as_deref_mut()
            }
        };
    }

    pub fn pop(&mut self) -> Option<T> {
        self.head.take().map(|head| {
            let head = *head;
            self.head = head.next;
            if self.head.is_none() {
                self.tail = None;
            }
            head.elem
        })
    }
}

// mod test {
//     use super::List;
//     #[test]
//     fn basics() {
//         let mut list = List::new();

//         // Check empty list behaves right
//         assert_eq!(list.pop(), None);

//         // Populate list
//         list.push(1);
//         list.push(2);
//         list.push(3);

//         // Check normal removal
//         assert_eq!(list.pop(), Some(1));
//         assert_eq!(list.pop(), Some(2));

//         // Push some more just to make sure nothing's corrupted
//         list.push(4);
//         list.push(5);

//         // Check normal removal
//         assert_eq!(list.pop(), Some(3));
//         assert_eq!(list.pop(), Some(4));

//         // Check exhaustion
//         assert_eq!(list.pop(), Some(5));
//         assert_eq!(list.pop(), None);
//     }
// }
