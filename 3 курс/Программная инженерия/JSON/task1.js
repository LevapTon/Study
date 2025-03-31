const _ = require("lodash")
const users = require("./json/users.json")

_
    .filter(users, (user) => {  // фильтр по южному полушарию
          return parseFloat(user.address.geo.lat) < 0;
        })
    .map(user => { // формализация файла
        return {
          username: user.username,
          city: user.address.city
        }
    })
    .sort((a, b) => {   // сортировка по городам по убыванию
        const cityA = a.city.toLowerCase();
        const cityB = b.city.toLowerCase();
        return cityA > cityB? +1: -1;
        // return (cityA < cityB) - (cityB < cityA)
      })
    .forEach(x => console.log(x)) // вывод
