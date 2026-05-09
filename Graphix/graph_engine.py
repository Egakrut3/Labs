#!/usr/bin/env python3

import argparse
import csv
import json
import math
import re
import statistics
import sys
from pathlib import Path
from xml.sax.saxutils import escape


COLORS = (
	"#2563eb",
	"#dc2626",
	"#16a34a",
	"#9333ea",
	"#ea580c",
	"#0891b2",
	"#be123c",
	"#4f46e5",
)


def try_float(value):
	try:
		return float(value)
	except (TypeError, ValueError):
		return None


def extract_x_from_command(command, fallback):
	values = [int(value) for value in re.findall(r"\b\d+\b", command)]
	values = [value for value in values if value >= 1000]
	if values:
		return float(max(values))

	return float(fallback)


def load_hyperfine_json(path):
	with open(path, "r", encoding="utf-8") as file:
		data = json.load(file)

	points = []
	for index, result in enumerate(data.get("results", []), 1):
		command = str(result.get("command", index))
		mean = try_float(result.get("mean"))
		stddev = try_float(result.get("stddev"))
		if mean is None:
			continue

		points.append({
			"x": extract_x_from_command(command, index),
			"y": mean,
			"err": stddev,
			"label": command,
		})

	return points


def load_csv(path):
	with open(path, "r", encoding="utf-8", newline="") as file:
		sample = file.read(4096)
		file.seek(0)
		dialect = csv.Sniffer().sniff(sample, delimiters=",;\t ")
		has_header = csv.Sniffer().has_header(sample)

		if has_header:
			rows = list(csv.DictReader(file, dialect=dialect))
			headers = rows[0].keys() if rows else ()
			lower_headers = {header.lower(): header for header in headers}
			x_key = lower_headers.get("x") or lower_headers.get("n") or next(iter(headers), None)
			y_key = lower_headers.get("y") or lower_headers.get("mean") or lower_headers.get("time")

			if y_key is None:
				for header in headers:
					if header != x_key:
						y_key = header
						break

			points = []
			for index, row in enumerate(rows, 1):
				x = try_float(row.get(x_key)) if x_key else None
				y = try_float(row.get(y_key)) if y_key else None
				err = try_float(row.get("stddev")) or try_float(row.get("err"))
				if y is None:
					continue

				points.append({
					"x": x if x is not None else float(index),
					"y": y,
					"err": err,
					"label": str(row.get(x_key, index)),
				})

			return points

		reader = csv.reader(file, dialect=dialect)
		points = []
		for index, row in enumerate(reader, 1):
			values = [try_float(cell) for cell in row]
			values = [value for value in values if value is not None]
			if not values:
				continue

			if len(values) == 1:
				x, y = float(index), values[0]
			else:
				x, y = values[0], values[1]

			points.append({"x": x, "y": y, "err": None, "label": str(index)})

		return points


def load_points(path):
	suffix = path.suffix.lower()
	if suffix == ".json":
		return load_hyperfine_json(path)
	if suffix in (".csv", ".tsv", ".txt", ".dat"):
		return load_csv(path)

	raise ValueError(f"Unknown input format: {path}")


def nice_step(value):
	if value <= 0:
		return 1.0

	exp = math.floor(math.log10(value))
	base = value / 10 ** exp

	if base <= 1:
		nice = 1
	elif base <= 2:
		nice = 2
	elif base <= 5:
		nice = 5
	else:
		nice = 10

	return nice * 10 ** exp


def make_ticks(min_value, max_value, count=6):
	if math.isclose(min_value, max_value):
		padding = abs(min_value) * 0.1 or 1.0
		min_value -= padding
		max_value += padding

	step = nice_step((max_value - min_value) / max(1, count - 1))
	start = math.floor(min_value / step) * step
	end = math.ceil(max_value / step) * step

	ticks = []
	value = start
	while value <= end + step * 0.5:
		ticks.append(value)
		value += step

	return ticks


def fmt_num(value):
	if abs(value) >= 1000:
		return f"{value:.0f}"
	if abs(value) >= 10:
		return f"{value:.1f}".rstrip("0").rstrip(".")
	return f"{value:.3f}".rstrip("0").rstrip(".")


def scale(value, min_value, max_value, left, right):
	if math.isclose(min_value, max_value):
		return (left + right) / 2

	return left + (value - min_value) * (right - left) / (max_value - min_value)


def median_step(values):
	values = sorted(set(values))
	if len(values) < 2:
		return 1.0

	return statistics.median(values[i] - values[i - 1] for i in range(1, len(values)))


def write_summary(series, output):
	output.parent.mkdir(parents=True, exist_ok=True)
	with open(output, "w", encoding="utf-8", newline="") as file:
		writer = csv.writer(file)
		writer.writerow(("series", "x", "mean", "stddev", "label"))
		for item in series:
			for point in sorted(item["points"], key=lambda cur: cur["x"]):
				writer.writerow((item["name"], fmt_num(point["x"]), point["y"], point["err"] or "", point["label"]))


def graph_svg(series, output, title, x_label, y_label, width, height):
	margin_left = 78
	margin_right = 28
	margin_top = 54
	margin_bottom = 76

	plot_left = margin_left
	plot_right = width - margin_right
	plot_top = margin_top
	plot_bottom = height - margin_bottom

	all_points = [point for item in series for point in item["points"]]
	if not all_points:
		raise ValueError("No points to draw")

	x_min = min(point["x"] for point in all_points)
	x_max = max(point["x"] for point in all_points)
	y_min = min(point["y"] for point in all_points)
	y_max = max((point["y"] + (point["err"] or 0.0)) for point in all_points)

	x_pad = median_step(point["x"] for point in all_points) * 0.5
	y_pad = (y_max - y_min) * 0.08 or 1.0
	x_min -= x_pad
	x_max += x_pad
	y_min = max(0.0, y_min - y_pad)
	y_max += y_pad

	x_ticks = make_ticks(x_min, x_max)
	y_ticks = make_ticks(y_min, y_max)

	def x_pos(value):
		return scale(value, x_min, x_max, plot_left, plot_right)

	def y_pos(value):
		return scale(value, y_min, y_max, plot_bottom, plot_top)

	lines = [
		f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
		"<style>",
		"text { font-family: Arial, sans-serif; fill: #111827; }",
		".small { font-size: 12px; fill: #4b5563; }",
		".label { font-size: 14px; fill: #374151; }",
		".title { font-size: 22px; font-weight: 700; }",
		".grid { stroke: #e5e7eb; stroke-width: 1; }",
		".axis { stroke: #111827; stroke-width: 1.5; }",
		"</style>",
		'<rect width="100%" height="100%" fill="#ffffff"/>',
		f'<text class="title" x="{width / 2:.1f}" y="30" text-anchor="middle">{escape(title)}</text>',
	]

	for tick in y_ticks:
		y = y_pos(tick)
		lines.append(f'<line class="grid" x1="{plot_left}" y1="{y:.2f}" x2="{plot_right}" y2="{y:.2f}"/>')
		lines.append(f'<text class="small" x="{plot_left - 10}" y="{y + 4:.2f}" text-anchor="end">{fmt_num(tick)}</text>')

	for tick in x_ticks:
		x = x_pos(tick)
		lines.append(f'<line class="grid" x1="{x:.2f}" y1="{plot_top}" x2="{x:.2f}" y2="{plot_bottom}"/>')
		lines.append(f'<text class="small" x="{x:.2f}" y="{plot_bottom + 20}" text-anchor="middle">{fmt_num(tick)}</text>')

	lines.extend([
		f'<line class="axis" x1="{plot_left}" y1="{plot_bottom}" x2="{plot_right}" y2="{plot_bottom}"/>',
		f'<line class="axis" x1="{plot_left}" y1="{plot_top}" x2="{plot_left}" y2="{plot_bottom}"/>',
		f'<text class="label" x="{(plot_left + plot_right) / 2:.1f}" y="{height - 24}" text-anchor="middle">{escape(x_label)}</text>',
		f'<text class="label" transform="translate(22 {(plot_top + plot_bottom) / 2:.1f}) rotate(-90)" text-anchor="middle">{escape(y_label)}</text>',
	])

	legend_x = plot_left + 12
	legend_y = plot_top + 20

	for index, item in enumerate(series):
		color = COLORS[index % len(COLORS)]
		points = sorted(item["points"], key=lambda point: point["x"])
		path = " ".join(f'{x_pos(point["x"]):.2f},{y_pos(point["y"]):.2f}' for point in points)

		lines.append(f'<polyline points="{path}" fill="none" stroke="{color}" stroke-width="2.5" stroke-linejoin="round" stroke-linecap="round"/>')

		for point in points:
			x = x_pos(point["x"])
			y = y_pos(point["y"])
			if point["err"]:
				y_low = y_pos(point["y"] - point["err"])
				y_high = y_pos(point["y"] + point["err"])
				lines.append(f'<line x1="{x:.2f}" y1="{y_low:.2f}" x2="{x:.2f}" y2="{y_high:.2f}" stroke="{color}" stroke-width="1.2"/>')
				lines.append(f'<line x1="{x - 4:.2f}" y1="{y_low:.2f}" x2="{x + 4:.2f}" y2="{y_low:.2f}" stroke="{color}" stroke-width="1.2"/>')
				lines.append(f'<line x1="{x - 4:.2f}" y1="{y_high:.2f}" x2="{x + 4:.2f}" y2="{y_high:.2f}" stroke="{color}" stroke-width="1.2"/>')

			lines.append(f'<circle cx="{x:.2f}" cy="{y:.2f}" r="4" fill="#ffffff" stroke="{color}" stroke-width="2"/>')

		lines.append(f'<line x1="{legend_x}" y1="{legend_y + index * 20}" x2="{legend_x + 24}" y2="{legend_y + index * 20}" stroke="{color}" stroke-width="2.5"/>')
		lines.append(f'<text class="small" x="{legend_x + 32}" y="{legend_y + 4 + index * 20}">{escape(item["name"])}</text>')

	lines.append("</svg>")
	output.parent.mkdir(parents=True, exist_ok=True)
	output.write_text("\n".join(lines) + "\n", encoding="utf-8")


def parse_series(values):
	series = []
	for value in values:
		if "=" in value:
			name, path = value.split("=", 1)
		else:
			path = value
			name = Path(path).stem

		points = load_points(Path(path))
		series.append({"name": name, "points": points})

	return series


def main(argv):
	parser = argparse.ArgumentParser(description="Draw a simple SVG line graph from CSV or hyperfine JSON.")
	parser.add_argument("inputs", nargs="+", help="Input files. Use name=path to set a series name.")
	parser.add_argument("-o", "--output", default="graph.svg", help="Output SVG path.")
	parser.add_argument("--summary-output", default=None, help="Optional summary CSV path.")
	parser.add_argument("--title", default="Benchmark results", help="Graph title.")
	parser.add_argument("--x-label", default="n", help="X axis label.")
	parser.add_argument("--y-label", default="time, s", help="Y axis label.")
	parser.add_argument("--width", type=int, default=960, help="SVG width.")
	parser.add_argument("--height", type=int, default=560, help="SVG height.")
	args = parser.parse_args(argv)

	series = parse_series(args.inputs)
	if args.summary_output:
		write_summary(series, Path(args.summary_output))
	graph_svg(series, Path(args.output), args.title, args.x_label, args.y_label, args.width, args.height)
	return 0


if __name__ == "__main__":
	raise SystemExit(main(sys.argv[1:]))
