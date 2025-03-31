Array.prototype.GetIndexElementBin = function (elem) {
    var left = 0
    var middle = 0
    var right = this.length-1
    while(left <= right) {
        middle = Math.floor((left + right) / 2)
        num = this[middle]
        if (num < elem) {
            left = middle + 1
        }
        else if (middle > elem) {
            right = middle - 1
        }
        else {
            return middle
        }
    }
    return(-1)
}


Array.prototype.GetIndexElementRec = function (elem, left=0, right=this.length-1) {
    let middle = Math.floor((left + right) / 2)
    num = this[middle]
    if (left > right) {
        return -1
    }
    if (num == elem) {
        return middle
    }
    if (num < elem) {
        left = middle + 1
        return this.GetIndexElementRec(elem, left, right)
    } else {
        right = middle
        return this.GetIndexElementRec(elem, left, right)
    }
}


var arr = new Array()
var search = 5
for(var i=0; i<10; i++) {
    arr[i] = i
}
console.log(arr)
console.log(arr.GetIndexElementBin(search))
console.log(arr.GetIndexElementRec(search))
