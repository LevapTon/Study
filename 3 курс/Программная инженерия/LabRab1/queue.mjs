export class Queue {
    constructor() {
        this.array = []
    }

    print = () => { // распечатать в консоль массив
        console.table(this.array)
        console.log()
    }

    enqueue = (value) => { // добавить в конец
        this.array.push(value)
    }

    dequeue = () => { // снять с конца и вернуть last

        let last = this.array[0]
        this.array.splice(0, 1)
        return last
    }

    peek = () => { // вернуть с конца, не меняя очередь
        return this.array[0]
    }

    size = () => { // вернуть размер
        return this.array.length
    }
}

