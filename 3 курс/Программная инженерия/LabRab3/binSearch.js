// const { Console } = require("console")
const fs = require("fs")


const GetWordsArray = (path) => {
    var arr = fs
        .readFileSync(path, 'utf8')
        .toLowerCase()
        .split('\n')
        .sort()
    return arr
}


const GetIndexElement = (arr, elem, left=0, right=arr.length-1, count=0) => {
    middle = Math.floor((left + right) / 2)
    word = arr[middle]
    // console.log(middle, num)
    if (left > right) {
        console.log('Кол-во вызовов:', count)
        return(-1)
    }
    if (word == elem) {
        console.log('Кол-во вызовов:', count)
        return(middle + 1)
    }
    else if (word < elem) {
        left = middle + 1
        return GetIndexElement(arr, elem, left, right, ++count)
    }
    else if (word > elem) {
        right = middle
        return GetIndexElement(arr, elem, left, right, ++count)
    }
}


var path_1 = './words_utf.txt'
var path_2 = './wanted_words.txt'
var dict = GetWordsArray(path_1)
var quest = GetWordsArray(path_2)
console.log(quest)

for (elem of quest) {
    console.log('Поиск слова:', elem)
    console.log('Позиция в словаре:', GetIndexElement(dict, elem), '\n')
}
