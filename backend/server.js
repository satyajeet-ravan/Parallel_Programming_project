import express from "express";import cors from "cors";import path from "path";import {fileURLToPath} from "url";
import imageRoutes from "./routes/imageRoutes.js";import benchmarkRoutes from "./routes/benchmarkRoutes.js";import systemRoutes from "./routes/systemRoutes.js";
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),"..");const app=express();
app.use(cors({origin:process.env.FRONTEND_ORIGIN||"http://localhost:5173"}));app.use(express.json());
app.use("/files",express.static(path.join(root,"images")));app.use("/api/image",imageRoutes);app.use("/api",benchmarkRoutes);app.use("/api",systemRoutes);
app.get("/api/health",(_,r)=>r.json({success:true,status:"ok"}));app.use((e,_,r,__)=>(console.error(e),r.status(e.status||500).json({success:false,error:e.message||"Server error"})));
app.listen(process.env.PORT||5000,()=>console.log("API listening on port "+(process.env.PORT||5000)));
