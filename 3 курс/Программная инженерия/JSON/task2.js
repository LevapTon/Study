const _ = require("lodash")
const data = require("./json/clients.json")


// _.(arr, ).value()
// _(arr).().()

// +1 0 -1 стабильная сортировка

const ex_01 = () => {
    let arr = _(data.clients)
        .filter(['address.city', "Кунгур"])
        .map(client => ({
            name: client.name,
            age: client.age,
            gender: client.gender,
            city: client.address.city
        }))
        .value()
        
        arr.sort((a,b) => a.name < b.name? +1: (a.name > b.name? -1: 0))
        arr.sort((a,b) => a.age > b.age? +1: (a.age < b.age? -1: 0))
        arr.sort((a,b) => a.gender < b.gender? +1: (a.gender > b.gender? -1: 0))
        
        
        // let f = (a,b) => {
        //     if (a.gender == b.gender) return a.age - b.age
        // }
        // arr.sort((a,b) => f(a,b))


    console.log(JSON.stringify(arr, ["gender", "age", "name"], 4))
}

const ex_02 = () => {
    _(data.clients)
        .filter(['address.city', "Кунгур"])
        .map(client => ({
            name: client.name,
            age: client.age,
            gender: client.gender,
            city: client.address.city
        }))
        .orderBy(["gender", "age", "name"], ["asc", "desc", "asc"])
        // .value()
        // .sort((a, b) => { 
        //     const genderA = a.gender
        //     const genderB = b.gender
        //     const ageA = a.age
        //     const ageB = b.age
        //     const nameA = a.name.toLowerCase()
        //     const nameB = b.name.toLowerCase()
        //     return (genderB < genderA) - (genderA < genderB) ||  // пол по возрастанию (f < m)
        //         (ageA < ageB) - (ageB < ageA) ||  // возраст по убыванию
        //         (nameB < nameA) - (nameA < nameB)  // имя по возрастанию
        //   })
        .forEach(x => console.log(x))
}

ex_01()
