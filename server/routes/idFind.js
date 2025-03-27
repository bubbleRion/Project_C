// 모듈연결
const express = require('express');
const router = express.Router();
const path = require("path");
// db 연결
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");
const info = require("../template/Info.js")

// idFind입력창 html 불러서 내보내줘
router.get("/", (req, res)=>{
    res.sendFile(path.join(__dirname, "../html", "idFind.html"))
})

router.post("/", (req,res)=>{
    // body는 리퀘스트 바디라고 적어줌
    let body = req.body
    // user db 접근
    db.query(`select * from ${info.table.user}`, (err, results)=>{
        if(err) console.error(err)
        let result = results.map((item)=>{
            // 만약 이름하고 이메일을 입력한 값이 유저 db에서 일치하는 값이 있다면 아이디를 반환해줘
            if(body.name == item.name && body.email == item.email){
                return item.id
            }
        })
        // 아닌 경우에는 정의 되지 않았음이니 걸러줘
        let idResult = result.filter((item)=>{
            return item !== undefined
        })
        res.send(`<h1>아이디는 ${idResult[0]}입니다.</h1><a href="/">메인으로 가기</a>`)
    })
})
module.exports = router