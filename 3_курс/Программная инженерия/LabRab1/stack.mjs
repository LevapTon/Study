export class Stack {
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
        return this.array[-1]
    }

    size = () => { // вернуть размер
        return this.array.length
    }
}

module.exports = {
    Stack
}
