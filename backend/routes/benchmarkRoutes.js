import {Router} from "express";import {getAll,summary} from "../controllers/benchmarkController.js";const r=Router();r.get("/benchmark",getAll);r.get("/benchmark/summary",summary);export default r;
