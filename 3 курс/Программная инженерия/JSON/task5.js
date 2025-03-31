const _ = require("lodash")
const data = require("./data")


let arr = _.zip(data.colors, data.argb)

let res = _.map(arr, (elem) => {
        const [color, argb] = elem
        const hex = '#' + _
            .map(argb, (value) => value.toString(16).padStart(2, '0'))
            .join('')
        return {color, hex_name: hex}
    })
    .sort((a, b) => {
        const colorA = a.color.toLowerCase()
        const colorB = b.color.toLowerCase()
        return (colorB < colorA) - (colorA < colorB)
})

res.forEach(x => console.log(x))
