const express = require('express')
const app = express()
const secretInfo = require("../private/process/protect.js")
const session = require('express-session')
const port = secretInfo.port

// express 바디파서 사용
app.use(express.json())
app.use(express.urlencoded({extended : true}))


// public경로 지정
app.use(express.static(__dirname + "/css"));
// 세션 사용
app.use(session(secretInfo.session))

//페이지 가져오기
const mainPage = require("./routes/index.js")
const boardPage = require("./routes/board.js")


// 페이지 경로 지정
app.use("/", mainPage)
app.use("/board" , boardPage)

app.get('/cake', (req, res) => {
    res.send('석준아 옷에 뭐 묻었다야');
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})