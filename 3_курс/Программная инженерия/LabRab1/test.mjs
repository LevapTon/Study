import { Stack } from './stack.mjs'
import { Queue } from "./queue.mjs"


// var arr = new Stack()
// console.log('Стек')
// for (var i=0; i < 10; i++) {
//     arr.push(i)
// }
// arr.print()
// console.log('Посмотреть последний:', arr.peek())
// arr.print()
// console.log('Размер:', arr.size())
// console.log('Изъять последний', arr.pop())
// console.log('Размер:', arr.size())
// arr.print()
// console.log('\n')

// var qu = new Queue()
// console.log('Очередь')
// for (var i=0; i < 10; i++) {
//     qu.enqueue(i)
// }
// qu.print()
// console.log('Посмотреть последний:', qu.peek())
// qu.print()
// console.log('Размер:', qu.size())
// console.log('Изъять последний', qu.dequeue())
// console.log('Размер:', qu.size())
// qu.print()
// console.log('\n')


const brackets = {
    '(': ')',
    '{': '}',
    '[': ']'
};
const closet_brackets = [')','}',']']
let line = ')((()))';
const stack = new Stack();

for (const char of line) {
    /* ↓ Добавление в стек открывающей скобки ↓ */
    if (brackets[char]) {
        stack.push(char);
        /* ↓ Проверка символа на то, является ли он закрывающей скобкой ↓ */
    } else if (char in closet_brackets) {
        /* ↓ Проверка на закрытие скобки ↓ */
        if (brackets[stack.peek()] === char) {
            stack.pop();
        /* ↓ Выход, если не та скобка ↓ */
        } else {
            break;
        }
    }
}

if (stack.size() === 0) {
    console.log('Success');
} else {
    console.log('Not Success');
}
