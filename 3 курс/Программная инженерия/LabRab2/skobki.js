class Stack {
    constructor() {
        this.array = []
    }

    print = () => { // распечатать в консоль массив
        console.table(this.array)
        console.log()
    }

    push = (value) => { // добавить на вершину
        this.array.push(value)
    }

    pop = () => { // снять с вершины и вернуть last
        return this.array.pop()
    }

    peek = () => { // вернуть с вершины, не меняя стек
        return this.array[this.array.length - 1]
    }

    size = () => { // вернуть размер
        return this.array.length
    }
}


// () success
// (){}[] success
// ({[]}) success
// e = [i for i in range(5]) not success

const brackets = {
    '(': ')',
    '{': '}',
    '[': ']'
};
const closet_brackets = [')','}',']']
let line = ')()'
const stack = new Stack()

for (const char of line) {
    /* ↓ Добавление в стек открывающей скобки ↓ */
    if (brackets[char]) {
        stack.push(char)
        /* ↓ Проверка символа на то, является ли он закрывающей скобкой ↓ */
    } else if (closet_brackets.includes(char)) {
        /* ↓ Проверка на закрытие скобки ↓ */
        if (brackets[stack.peek()] === char) {
            stack.pop()
        } else {
            console.log('Not Success')
            break
        }
    }
    if (char == line[line.length - 1]) {
        if (stack.size() === 0) {
            console.log('Success');
        } else {
            console.log('Not Success');
        }
    }
}
