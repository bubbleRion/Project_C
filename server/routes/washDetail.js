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
let time = 5
let future = 0
let cake = 1
router.get("/:id", (req,res)=>{
    let userId = req.session.userID
    let index = req.params.id
    if(userId){
        db.query(`select id , is_working , is_switch , dormitoryroom , phonenumber , timeout from ${info.table.wash} where id = ${index}`, (err, results)=>{
            if(err) console.error(err)
            results.forEach((item)=>{
                let id = item.id
                let domi = item.dormitoryroom
                let phone = item.phonenumber
                let is_working = item.is_working
                let is_switch = item.is_switch
                let timeout = item.timeout 
                let now = Date.now()
                if(is_switch == 1){
                    if(cake == 1){
                        now = Date.now()
                    }
                        if(future - now >= 20000 || time <= 1){
                            future = Date.now()
                            console.log(future - now + "cake")
                            timeout = ""
                            db.query(`update ${info.table.wash} set is_working = 0 , is_switch = 0 , dormitoryroom = NULL, phonenumber = NULL , timeout = 0 where id = ${index};` , (err)=>{
                                if(err) console.error(err)
                            }) // reset update end
                            time = 5
                            cake = 0
                        }
                        else{
                            console.log(future)
                            time -= 1
                            timeout = time
                            db.query(`update ${info.table.wash} set timeout = ${timeout} where id = ${index};` , (err)=>{
                                if(err) console.error(err)
                            }) // update2 end
                            future = Date.now()
                            if(time == 5){
                                cake = 1
                            }
                            
                        }
                    }
                    res.send(literal.washDetail(id , is_working , is_switch , domi , phone , userId , timeout , is_workings_join = ""))
                })
            })
        }
        else{
            res.redirect("/")
        }
    })



module.exports = router