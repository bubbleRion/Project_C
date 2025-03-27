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
    // 로그인 텍스트
    let text = info.data.text
    // 비회원 전용 글쓰기 버튼 텍스트
    let writeText = info.data.writeText
    if(userId){
        // 로그아웃 텍스트
        text = info.data.userText
        // 유저 전용 글쓰기 버튼 텍스트
        writeText = info.data.userWriteText
    
    // db열어주기
        db.query(`select * from ${info.table.board}`, (err, results)=>{
            // 에러 체크해주기
            if(err) console.error(err);
            // db의 결과를 리버스 메소드와 map 메소드 사용해서 이미지만 출력해주기
            let result = results.reverse().map((item)=>{
                return `<a href="board${item.seq}"><img src="${item.image.replace("s", "s/")}" alt=""/><a>
                `
            })
            // 문자열로 합쳐주기
            let boardData = result.join("")
            // 템플릿에서 불러온 html스트링에 가공된 데이터 넣어주기
            res.send(literal.boardTemp(text, writeText, boardData))
        })
    }
    else{
        res.sendFile(path.join(__dirname  ,"../html" , "index.html"))
    }
    
})
module.exports = router