class Node {
    constructor(value) {
        this.key = value
        this.left = null
        this.right = null
    }
}


const CreateTree = (arr = new Array) => {
    var new_arr = new Array()
    for (item of arr) {
        new_arr.push(item)
        pos = new_arr.length - 1
        while(pos > 0) {
            pos_par = Math.floor((pos - 1) / 2)
            if (new_arr[pos_par] < new_arr[pos]) {
                term = new_arr[pos_par]
                new_arr[pos_par] = new_arr[pos]
                new_arr[pos] = term
            }
            else {
                break
            } 
            pos = pos_par
        }
    }
    return new_arr
}


const SortTree = (arr = new Array) => {
    for (var i = arr.length-1; i != 0; i--) {
        term = arr[0]
        arr[0] = arr[i]
        arr[i] = term
        par = 0
        while (par < i) {
            child_1 = par * 2 + 1
            child_2 = par * 2 + 2
            if (child_2 < i && arr[child_2] > arr[par] && arr[child_2] >= arr[child_1]) {
                term = arr[par]
                arr[par] = arr[child_2]
                arr[child_2] = term
                par = child_2
            }
            else if (child_1 < i && arr[child_1] < arr[par]) {
                term = arr[par]
                arr[par] = arr[child_1]
                arr[child_1] = term
                par = child_1
            }
            else {
                break
            }
        }
    }
    return arr
}


const GetTree = (arr = new Array) => {
    // arr = CreateTree(arr)
    // arr = SortTree(arr)
    // console.log(arr)
    
    // var arr_nodes = new Array()
    // for (var i = 0; i < arr.length; i++) {
    //     arr_nodes.push(new Node(arr[i]))
    // }
    // for (var i = 0; i < arr_nodes.length - 2; i++) {
    //     if (2*i + 2 < arr_nodes.length) {
    //         if (arr_nodes[2*i+1] <= arr_nodes[i] && arr_nodes[i+1] < arr_nodes[2*i+2]) {
    //             arr_nodes[i].left = arr_nodes[2*i+1]
    //             arr_nodes[i].right = arr_nodes[2*i+2]
    //         }
    //         else {
    //             arr_nodes[i].left = arr_nodes[2*i+2]
    //             arr_nodes[i].right = arr_nodes[2*i+1]
    //         }
    //     }
    //     else if (2*i + 1 < arr_nodes.length) {
    //         if (arr_nodes[2*i+1] <= arr_nodes[i]) {
    //             arr_nodes[i].left = arr_nodes[2*i+1]
    //         }
    //         else {
    //             arr_nodes[i].right = arr_nodes[2*i+1]
    //         }
    //     }   
    // }
    // return arr_nodes[0]

    // for (var i = 0; i < arr_nodes.length; i++) {
    //     console.log(arr_nodes[i])
    //     console.log(arr_nodes[i].left, arr_nodes[i].right)
    // }
}


const SearchInTree = (node, elem) => {
    if (node.key == elem) {
        console.log('end')
        return(node.key)
    }
    if (node.key >= elem) {
        console.log(node.key)
        if (node.left !== null) {
            console.log('left')
            return SearchInTree(node.left, elem)
        }
        else {
            console.log('end left')
            return
        }
    }
    else if (node.key < elem) {
        console.log(node.key)
        if (node.right !== null) {
            console.log('right')
            return SearchInTree(node.right, elem)
        }
        else {
            console.log('end right')
            return
        }
    }
    else {
        console.log('bad end')
        return null
    }
}


var arr = Array(10)
    .fill(0)
    .map(x => Math.floor(10 * Math.random()))
search = arr[5]
console.log('Исходный массив:\n', arr, '\n')
GetTree(arr)
// var elem = GetTree(arr)
// // console.log(elem)
// console.log('Искомое число:', search, '\n')
// console.log('\n', SearchInTree(elem, search))
