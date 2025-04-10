// 모듈연결
const express = require('express');
const router = express.Router();
const path = require('path');
// db 연결
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);
// info와 템플릿 연결
const info = require("../template/Info.js")
const literal = require("../template/literal.js");
router.get("/", (req, res)=>{
    
    // userId에 세션에 저장된 아이디 불러오기
    let userId = req.session.userID
    // userId = false // 세션기능 추가후 수정

    if(userId){
        db.query(`select id , is_working from ${info.table.wash}`, (err, results)=>{
            if(err) console.error(err)
            let is_workings = results.map((item)=>{
                let is_working = item.is_working
                return is_working
            })
            let is_workings_join = is_workings.join("")
            res.send(literal.indexTemp(is_workings_join))
        })

    }
    else{
        res.sendFile(path.join(__dirname  ,"../html" , "index.html"))
    }
    
})
module.exports = router