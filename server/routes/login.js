// 모듈연결
const express = require('express');
const router = express.Router();
// db 연결
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");
const info = require("../template/Info.js")
const path = require("path")
const {check , validationResult} = require("express-validator")
const literal = require("../template/literal.js")
// 비밀번호 감추기 시작
const crypto = require("crypto");
// 세션 데이타 왜 빈 객체인지 사실 잘 모르겠어요
const sessionData = {}

// 로그인 페이지 마크업 연결
router.get("/", (req, res)=>{
    res.sendFile(path.join(__dirname  ,"../html" , "loginPage.html"))
})

router.post("/", [
    // username은 이메일이어야 한다.
    check('userId').isLength({min : 5 , max : 12}),
    // 비밀번호는 5글자 이상이어야 한다.
    check('password').isLength({ min: 4 , max : 12})
  ], (req,res)=>{
    // validate check
    const errors = validationResult(req);
    // 만약 errors가 비어있지 않다면 (errors에 err가 존재한다면)
    if (!errors.isEmpty()) {
    	// 첫 번째 에러를 firstError 변수에 할당
        const firstError = errors.array()[0]
        // firstError의 msg 프로퍼티를 json으로 보내줌
        return res.status(400).json(firstError.msg);
    }

    // 로그인 여부 체크
    let isLogin = false
    // 관리자 여부 체크
    let isAdmin = false
    // 바디는?
    let body = req.body
    // 편하려고 바디에서 아이디 꺼냄
    let id = body.userId
    // 위와 동일하게 비번 꺼냄
    let password = body.password
    // 유저 아이디 빈값으로 받아옴 코드 자체가 의미 없는 거 같기도 합니다.
    let userId = info.data.userId
    // 비밀번호를 user테이블에서 찾아요. 기준은 프라이머리 키인 아이디
    db.query(`select password from ${info.table.user} where userid ="${id}"`, (err, results)=>{
        // 아이디 없을 때
        if(results[0] == undefined){
            console.log("아이디 틀림")
        }
        // 관리자 일 때
        else if(id == "admin"){
            isAdmin = true
        }
        // 회원일 때
        else{
            // 비밀번호 암호화 소금 뿌리기
            let saltPw = "" + password * secretInfo.password.salt
            // 비밀번호 암호화 감자 만들기
            let hashPassword = crypto.createHash(secretInfo.password.hash).update(saltPw).digest(secretInfo.password.digest)
            // 뽑은 회원의 비밀번호와 암호화된 비밀번호 일치하면 로그인 여부 true로 해주고 userId에 아이디를 담아줌
            if(results[0].password === hashPassword){
                isLogin = true
                userId = id
            }
            // 아닐 경우에는 둘다 false로
            else{
                isLogin = false
                isAdmin = false
            }
        }
        // 로그인 체크가 true일 때
        if(isLogin){
            console.log("로그인 성공")
            // 세션에 userId넣기
            req.session.userID = userId
            // 세션 저장
            req.session.save()
            // 세션 시간 설정
            req.session.touch({maxAge : 90 * 60 * 1000})
            if(req.session){
                // 객체에 id와 password 세션화 해서 넣어주기
                sessionData[req.sessionID] = {id,password}
                // 쿠키와 세션 보내주기
                res.setHeader("Set-Cookie", `${req.sessionID}=${req.sessionID}; path=/`)
                .cookie("connect_id", `${req.sessionID}`, {maxAge : 90 * 60 * 1000})
                .redirect("/")
            }
        }
        // 관리자 계정 전용인데 없앨 예정
        else if(isAdmin){
            console.log("관리자입니다.")
            req.session.userID = userId
            req.session.save()
            req.session.touch({maxAge : 90 * 60 * 1000})
            res
            .setHeader("Set-Cookie", `${req.sessionID}=${req.sessionID}; path=/`)
            .cookie("connect_Admin", `${req.sessionID}`, { maxAge: 5000 })
            .redirect("/admin")
        }
        // 회원 , 관리자 아닐 경우
        else{
            console.log("로그인 실패")
            res.redirect("/login")
        }
    })
})
module.exports = router