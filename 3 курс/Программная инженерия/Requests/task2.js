const _ = require("lodash")
const request = require('request')


const solver = (url) => {
    request.get(url, (error, response, data) => {
        if (!error && response.statusCode == 200) {
            let count = 0
            let lines = data.split("\n")
            lines.forEach(x => {
                x = x.split(' ')
                row = _.size(x)
                for (let i = 0; i < row; i++) {
                    const num = parseInt(x[i])
                    if (num % 2 == 0) {
                        break
                    }
                    if (i == row - 1) {
                        count++
                    }
                }
            })
            console.log(count)
        }
    })
}


url = "https://pcoding.ru/txt/labrab04-2.txt"
solver(url)
