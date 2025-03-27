const express = require('express');
const router = express.Router();

// db 연결
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");
const info = require("../template/Info.js")
const literal = require("../template/literal.js");
const crypto = require("crypto");
const salt = secretInfo.password.salt

router.get("/",  (req, res)=>{
    let userId = req.session.userID
    let text = info.data.text
    if(userId){
        text = info.data.userId
    }
    res.send(literal.pwFindTemp(text))
})

router.post("/", (req,res)=>{
    let newPW = 1234
    let saltPw = "" + newPW * salt
    let hashPassword = crypto.createHash(secretInfo.password.hash).update(saltPw).digest(secretInfo.password.digest)
    db.query(`selct * from ${info.table.user}`, (err , results)=>{
        if(err) console.error(err)
        results.forEach(items =>{
            if(items.id == req.body.id && items.email == req.body.email){
                db.query(`UPDATE ${info.table.user} SET password= "${hashPassword}" WHERE id = "${req.body.id}"`, (err)=>{
                    if(err) console.error(err)
                })
            }
            else{
                console.log("회원 정보가 일치하지 않습니다.")
            }
        })
    })
    res.redirect("/")
})
module.exports = router
