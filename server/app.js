const express = require('express')
const app = express()
const helmet = require('helmet')

const secretInfo = require("../private/process/protect.js")
const session = require('express-session')
const port = secretInfo.port

app.use(helmet())
// express 바디파서 사용
app.use(express.json())
app.use(express.urlencoded({extended : true}))


// public경로 지정
app.use(express.static(__dirname + "/css"));
// 세션 사용
app.use(session(secretInfo.session))

//페이지 가져오기
const mainPage = require("./routes/index.js") //
const signUpPage = require("./routes/signup.js") //
const loginPage = require("./routes/login.js") //
const idFind = require("./routes/idFind.js") //
const pwFind = require("./routes/pwFind.js") //
const logout = require("./routes/logout.js") //

const washDetail = require("./routes/washDetail.js")

const update1 = require("./routes/update1.js")
const update3 = require("./routes/update3.js")
const update4 = require("./routes/update4.js")

// 페이지 경로 지정
app.use("/", mainPage)
app.use("/signup", signUpPage)
app.use("/login", loginPage)
app.use("/idFind", idFind)
app.use("/pwFind", pwFind)
app.use("/logout", logout)

app.use("/", washDetail)

app.use("/update1", update1)
app.use("/update3", update3)
app.use("/update4", update4)


app.get('/cake', (req, res) => {
    res.send('석준아 옷에 뭐 묻었다야');
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})