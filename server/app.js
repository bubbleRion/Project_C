const express = require('express')
const app = express()
const port = 8090 // wpqkf

app.get('/', (req, res) => {
    res.send('hello , 어진 , 해준 , 재윤');
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})