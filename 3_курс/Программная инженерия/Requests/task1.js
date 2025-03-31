const _ = require("lodash")
const request = require('request') // npm i request
// https://www.npmjs.com/package/request


const solver = (url) => {
    request.get(url, (error, response, data) => {
        if (!error && response.statusCode == 200) {
            let lines = data.split("\n")
            lines = _.filter(lines, (line) => {
                    const num = parseInt(line)
                    return num < 100
                })
                .sort((a, b) => {
                    const numA = parseInt(a)
                    const numB = parseInt(b)
                    return (numA < numB) - (numB < numA)
            })
            console.log(lines[0])
        }
    })
}

const solver2 = (url) => {
    const cb = (e, _, data) => {
        console.log(
            Math.max(
                ...data
                    .split('\n')
                    .filter(x => x.length === 2)
                    .map(x => +x)
            )
        )
    }
    request.get(url, cb)
}

url = "https://pcoding.ru/txt/labrab04-1.txt"
solver2(url)
