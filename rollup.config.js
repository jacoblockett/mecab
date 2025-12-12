import commonjs from "@rollup/plugin-commonjs"
import resolve from "@rollup/plugin-node-resolve"

export default [
	{
		input: "src/index.mjs",
		output: [
			{
				file: "dist/index.cjs",
				format: "cjs"
			},
			{
				file: "dist/index.mjs",
				format: "es"
			}
		],
		external: ["node-gyp-build", "path", "url", "module", "fs"],
		plugins: [resolve({ preferBuiltins: true }), commonjs()]
	}
]
