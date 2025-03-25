const express = require('express')
const app = express()
const port = 8090 // wpqkf

app.get('/', (req, res) => {
    res.send('석준아 나가라');
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})