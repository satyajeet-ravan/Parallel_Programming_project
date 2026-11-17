import {Router} from "express";import {info} from "../controllers/systemController.js";const r=Router();r.get("/system/info",info);export default r;
