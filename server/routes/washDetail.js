const path = require("path")
const express = require('express');
const router = express.Router();
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);
// info와 템플릿 연결
const info = require("../template/Info.js")
const literal = require("../template/literal.js");
// 주기적 is_timeout 체크
const schedule = require("node-schedule");

router.get("/:id", (req,res)=>{
    let userId = req.session.userID
    let index = req.params.id
    let idx = index[5]
    
        if(userId){
            db.query(`select id , is_working , is_switch , dormitoryroom , phonenumber from ${info.table.wash} where id = ${idx}`, (err, results)=>{
                if(err) console.error(err)
                results.forEach((item)=>{
                    let id = item.id
                    let domi = item.dormitoryroom
                    let phone = item.phonenumber
                    let is_working = item.is_working
                    let is_switch = item.is_switch
                    let is_timeout = 0
                    res.send(literal.washDetail(id ? id : idx, is_working , is_switch , is_timeout , domi , phone , userId))

                })
                
            })
        }
        
        
        
    })


module.exports = router