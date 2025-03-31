const SearchTarget = (sum, id_A, id_B) => {
    // if (nums.length == 0) {
    //     return [-1,-1]
    // }
    sum = nums[id_A] + nums[id_B]
    if (sum == target) {
        return  [id_A, id_B]
    }
    if (nums[id_B+1] && id_A != id_B) {
        sum = nums[id_A] + nums[id_B]
        return SearchTarget(sum, id_A, id_B + 1)
    }
    if (nums[id_A+1]  && id_A != id_B) {
        sum = nums[id_A] + nums[id_B]
        return SearchTarget(sum, id_A + 1, id_B)
    }
    return [id_A, id_B]
}


let nums = [3,3]
let target = 6
console.log(SearchTarget(0,0,1))