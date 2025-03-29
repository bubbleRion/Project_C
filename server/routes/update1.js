const express = require('express')
const router = express.Router();
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const info = require("../template/Info.js")
const literal = require("../template/literal.js");

router.post("/" , (req , res)=>{
    let userId = req.session.userID
    
    let seq = req.body.cake
    console.log(userId)
    let sql1 = `select id, is_switch , dormitoryroom , phonenumber from ${info.table.user} where userid = "${userId}"`
    db.query(sql1, (err , row)=>{
        if(err) console.error(err)
        row.forEach((item)=>{
            console.log(item)
            let domi = item.dormitoryroom
            let phone = item.phonenumber
            let is_switch = item.is_switch
            if(is_switch == 1){

                db.query(`update ${info.table.wash} set is_working = 1 , dormitoryroom = "${domi}" , phonenumber = "${phone}" where id = ${seq};` , (err)=>{
                    if(err){
                        console.error(err)
                    }
                    else{
                        console.log(domi + "caky")            
                    }
                }) // update end
            }
        })// select end
    })
    res.redirect(`index${seq}`)
    
    
})
module.exports = router