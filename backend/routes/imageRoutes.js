import {Router} from "express";import multer from "multer";import os from "os";import path from "path";import {upload,process,compare} from "../controllers/imageController.js";
const r=Router(),allowed=new Set([".jpg",".jpeg",".png",".bmp"]);const up=multer({dest:path.join(os.tmpdir(),"aie-upload"),limits:{fileSize:20*1024*1024},fileFilter:(_,f,cb)=>allowed.has(path.extname(f.originalname).toLowerCase())?cb(null,true):cb(Error("Only JPG, JPEG, PNG and BMP are allowed"))});
r.post("/upload",up.single("image"),upload);r.post("/process",process);r.post("/compare",compare);export default r;
