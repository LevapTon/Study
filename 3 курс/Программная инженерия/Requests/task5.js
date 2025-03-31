const _ = require("lodash")
const request = require('sync-request')

const get_lines_from_url = (url) => {
    let data = request("GET", url).getBody("utf8")
    return data.split("\n")
}

url = "https://pcoding.ru/txt/labrab04-3.txt"
let lines = get_lines_from_url(url)
lines = _.map(lines, (line) => {
            const rate = line.split(';')[0]
            const name = line.split(';')[1]
            return {
                rate: rate,
                name: name
            }
        })
        .sort((a, b) => {
            const rateA = parseFloat(a.rate.replace('%', '')) 
            const rateB = parseFloat(b.rate.replace('%', ''))
            return (rateA < rateB) - (rateB < rateA)
    })
lines.forEach(x => console.log(`${x.rate}\t${x.name}`))
