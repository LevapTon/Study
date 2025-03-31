class Queue {
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
        return this.array.shift()
    }

    peek = () => { // вернуть с начала, не меняя очередь
        return this.array[0]
    }

    size = () => { // вернуть размер
        return this.array.length
    }
}


const CountPrograms = (count, queue = new Queue()) => {
    if (queue.size() === 0) {
        return count
    }
    let current = queue.dequeue()
    if (current > Y) {
        return count
    }
    if (current == Y) {
        count += 1
    }
    queue.enqueue(current + A)
    count = CountPrograms(count, queue)
    queue.enqueue(current + B)
    count = CountPrograms(count, queue)
    return count
}


let lines = '1 18 1 2';
let line = lines.split(' ').map((elem) => parseInt(elem));
let X = parseInt(line[0])
let Y = parseInt(line[1])
let A = parseInt(line[2])
let B = parseInt(line[3])
let queue = new Queue();
let count = 0;
queue.enqueue(X);
count = CountPrograms(count, queue)
console.log(count);
