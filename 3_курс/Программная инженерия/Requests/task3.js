const _ = require("lodash")
const request = require('request')


const solver = (url) => {
    request.get(url, (error, response, data) => {
        if (!error && response.statusCode == 200) {
            let answ = 0
            let sum = 0
            let sumMax = 0
            let lines = data.split("\n")
            lines.forEach((x, index) => {
                x = x.split(' ')
                row = _.size(x)
                for (let i = 0; i < row; i++) {
                    const num = parseInt(x[i])
                    if (num % 2 == 0) {
                        sum = 0
                        break
                    }
                    sum += num
                    if (i == row - 1) {
                        if (sum >= sumMax) {
                            answ = index
                        }
                    }
                }
            })
            console.log(answ)
        }
    })
}


url = "https://pcoding.ru/txt/labrab04-2.txt"
solver(url)
