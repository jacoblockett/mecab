import { fileURLToPath } from "url"
import { dirname, join } from "path"
import { createRequire } from "module"
import { existsSync, readdirSync } from "fs"

const __filename = fileURLToPath(import.meta.url)
const __dirname = dirname(__filename)
const require = createRequire(import.meta.url)
const binding = require("node-gyp-build")(join(__dirname, ".."))
const engineDicts = {
	jp: join(__dirname, "..", "dict", "jp"),
	ko: join(__dirname, "..", "dict", "ko")
}
const JP = "jp"
const KO = "ko"
const EMPTY = "*"

export default class MeCab {
	#engine = JP
	#dictPath = engineDicts[JP]
	#tagger

	/**
	 * Creates a MeCab engine class that can parse specific text.
	 *
	 * @param {object} [opts]
	 * @param {"jp"|"ko"} [opts.engine] Which MeCab language engine to use. jp is the original MeCab, ko is the Korean patch. Default=`"jp"`
	 * @param {string} [opts.dictPath] The path to your custom compiled dictionary. Default=uses the engine dictionary
	 */
	constructor(opts) {
		if (Object.prototype.toString.call(opts) === "[object Object]") {
			if (typeof opts.engine === "string") {
				const engine = opts.engine.trim().toLowerCase()

				if (engineDicts[engine]) {
					this.#engine = engine
					this.#dictPath = engineDicts[engine]
				} else {
					throw new Error(`"${opts.engine}" is not a supported mecab engine.`)
				}
			}

			if (typeof opts.dictPath === "string") {
				this.#dictPath = opts.dictPath.trim()
			}
		}

		if (!existsSync(this.#dictPath)) throw new Error(`"${this.#dictPath}" doesn't exist.`)

		const expectedFiles = ["char.bin", "dicrc", "matrix.bin", "sys.dic", "unk.dic"]
		try {
			const foundFiles = readdirSync(this.#dictPath)

			for (const expectedFile of expectedFiles) {
				if (!foundFiles.includes(expectedFile)) throw ""
			}
		} catch (err) {
			throw new Error(
				`Ensure your dictionary path contains a compiled dictionary. The minimum viable contents should be ${expectedFiles.join(
					", "
				)}.`
			)
		}

		this.#tagger = new binding.Mecab(this.#engine, this.#dictPath)
	}

	/**
	 * Parses the given text into tokens.
	 *
	 * @param {string} text The text to parse.
	 * @returns {Token[]}
	 */
	parse(text) {
		const raw = this.#tagger.parse(text)
		const tokens = []

		for (const line of raw.split(/[\r\n]+/)) {
			if (line === "EOS") break

			const token = new Token(this.#engine, line)

			tokens.push(token)
		}

		return tokens
	}
}

class Token {
	#engine
	#surface
	#features

	/**
	 * A structure representing parsed data provided by MeCab.
	 *
	 * @param {"jp"|"ko"} engine The language engine used to parse the token
	 * @param {string} raw The raw surface and feature set of the token
	 */
	constructor(engine, raw) {
		if (engine !== JP && engine !== KO) {
			throw new Error(`"${engine}" is not a supported mecab engine.`)
		}

		this.#engine = engine

		const [surface, features] = raw.split("\t")

		this.#surface = surface
		this.#features = features.split(",")
	}

	get base() {
		if (this.#engine === KO) {
			let base = this.#surface

			if (this.#features[4] !== EMPTY && this.#features[7] !== EMPTY) {
				base = this.#features[7].split("/")[0]
			}

			return base
		}

		return null
	}

	get expression() {
		if (this.#engine === KO) {
			return this.#features[7] !== EMPTY ? this.#features[7].split("+").map(part => new ExpressionToken(part)) : null
		}

		return null
	}

	get features() {
		return this.#features
	}

	get hasBatchim() {
		if (this.#engine === KO) {
			return this.#features[2] === "T"
		}

		return null
	}

	get hasJongseong() {
		if (this.#engine === KO) {
			return this.#features[2] === "T"
		}

		return null
	}

	get pos() {
		if (this.#engine === KO) {
			return this.#features[0].split("+")
		}
	}

	get pronunciation() {
		if (this.#engine === KO) {
			return this.#features[3] !== EMPTY ? this.#features[3] : null
		}
	}

	get raw() {
		return `${this.#surface}\t${this.#features.join(",")}`
	}

	get reading() {
		if (this.#engine === KO) {
			return this.#features[3] !== EMPTY ? this.#features[3] : null
		}
	}

	get semanticClass() {
		if (this.#engine === KO) {
			return this.#features[1] !== EMPTY ? this.#features[1] : null
		}

		return null
	}

	get surface() {
		return this.#surface
	}

	get type() {
		if (this.#engine === KO) {
			return this.#features[4] !== EMPTY ? this.#features[4] : null
		}

		return null
	}
}

class ExpressionToken {
	#features

	constructor(raw) {
		this.#features = raw.split("/")
	}

	get morpheme() {
		return this.#features[0]
	}

	get pos() {
		return this.#features[1]
	}

	get semanticClass() {
		return this.#features[2] !== EMPTY ? this.#features[2] : null
	}
}
