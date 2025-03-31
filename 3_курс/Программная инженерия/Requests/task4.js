const _ = require("lodash")
const request = require('sync-request')

const get_lines_from_url = (url) => {
    let data = request("GET", url).getBody("utf8")
    return data.split("\n")
}

url = "https://pcoding.ru/txt/labrab04-3.txt"
let lines = get_lines_from_url(url)
lines = _.map(lines, (line) => {
            const res = line.split(';')[1]
            return res
        })
        .sort((a, b) => {
            const nameA = _.size(a) 
            const nameB = _.size(b)
            return (nameB < nameA) - (nameA < nameB)
    })
lines.forEach(x => console.log(x))
