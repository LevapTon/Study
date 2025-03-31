const _ = require("lodash")
const request = require('sync-request')

const get_lines_from_url = (url) => {
    let data = request("GET", url).getBody("utf8")
    return JSON.parse(data)
}

url = "https://pcoding.ru/json/abiturs.json"
let lines = get_lines_from_url(url)
lines = _.map(lines, (line) => {
    const lastName = line.lastName
    const rating = parseInt(line.rating)
    const city = line.city

    return {
        lastName: lastName,
        rating: rating,
        city: city
    }
})
.sort((a, b) => {
    const cityA = a.city.toLowerCase()
    const cityB = b.city.toLowerCase()
    const rateA = a.rating
    const rateB = b.rating

    return (cityB < cityA) - (cityA < cityB) ||
        (rateA < rateB) - (rateB < rateA)
})

lines.forEach(x => console.log(x))
