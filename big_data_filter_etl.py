#!/usr/bin/env python3
"""可测试的大数据过滤 ETL 示例。

特点：
- 使用 csv.DictReader 流式读取，不一次性把数据全部加载进内存，适合大文件。
- Extract: 从 CSV 文件或标准输入读取原始数据。
- Transform: 清洗字段、转换类型、标准化国家/状态。
- Load: 输出过滤后的 CSV，并在标准错误输出处理摘要。

示例：
    python big_data_filter_etl.py --input sample.csv --output result.csv --min-age 18 --min-amount 100
"""

from __future__ import annotations

import argparse
import csv
import sys
from contextlib import nullcontext
from dataclasses import dataclass
from pathlib import Path
from typing import ContextManager, IO, Iterable, Iterator


@dataclass(frozen=True)
class CleanRecord:
    """转换后的干净记录。"""

    user_id: str
    age: int
    country: str
    status: str
    amount: float


@dataclass
class ETLSummary:
    """ETL 处理统计，便于测试和观察运行结果。"""

    read_rows: int = 0
    valid_rows: int = 0
    filtered_rows: int = 0
    invalid_rows: int = 0


OUTPUT_FIELDS = ["user_id", "age", "country", "status", "amount"]
ACTIVE_STATUSES = {"active", "paid", "vip"}


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="流式大数据过滤 ETL 示例")
    parser.add_argument("--input", "-i", type=Path, help="输入 CSV 文件；省略时从 stdin 读取")
    parser.add_argument("--output", "-o", type=Path, help="输出 CSV 文件；省略时写入 stdout")
    parser.add_argument("--min-age", type=int, default=18, help="保留的最小年龄，默认 18")
    parser.add_argument("--min-amount", type=float, default=100.0, help="保留的最小交易金额，默认 100")
    parser.add_argument(
        "--country",
        action="append",
        default=[],
        help="只保留指定国家，可重复传入；省略则不过滤国家，如 --country CN --country US",
    )
    return parser.parse_args(argv)


def open_input(path: Path | None) -> ContextManager[IO[str]]:
    if path is None:
        return nullcontext(sys.stdin)
    return path.open("r", encoding="utf-8", newline="")


def open_output(path: Path | None) -> ContextManager[IO[str]]:
    if path is None:
        return nullcontext(sys.stdout)
    return path.open("w", encoding="utf-8", newline="")


def normalize_status(value: str) -> str:
    return value.strip().lower()


def normalize_country(value: str) -> str:
    return value.strip().upper()


def transform(row: dict[str, str]) -> CleanRecord:
    """将原始 CSV 行转换为强类型记录；字段缺失或格式错误时抛出 ValueError。"""

    user_id = row.get("user_id", "").strip()
    country = normalize_country(row.get("country", ""))
    status = normalize_status(row.get("status", ""))
    age_text = row.get("age", "").strip()
    amount_text = row.get("amount", "").strip()

    if not user_id:
        raise ValueError("user_id is required")
    if not country:
        raise ValueError("country is required")
    if not status:
        raise ValueError("status is required")

    age = int(age_text)
    amount = float(amount_text)

    if age < 0:
        raise ValueError("age must be non-negative")
    if amount < 0:
        raise ValueError("amount must be non-negative")

    return CleanRecord(user_id=user_id, age=age, country=country, status=status, amount=amount)


def should_keep(
    record: CleanRecord,
    *,
    min_age: int,
    min_amount: float,
    countries: set[str],
) -> bool:
    """业务过滤规则：活跃用户、年龄达标、交易金额达标、国家可选匹配。"""

    if record.status not in ACTIVE_STATUSES:
        return False
    if record.age < min_age:
        return False
    if record.amount < min_amount:
        return False
    if countries and record.country not in countries:
        return False
    return True


def extract(input_file: IO[str]) -> Iterator[dict[str, str]]:
    """从 CSV 输入流中逐行抽取数据。"""

    reader = csv.DictReader(input_file)
    yield from reader


def load(records: Iterable[CleanRecord], output_file: IO[str]) -> int:
    """将处理后的记录写成 CSV，返回写入行数。"""

    writer = csv.DictWriter(output_file, fieldnames=OUTPUT_FIELDS)
    writer.writeheader()
    count = 0
    for record in records:
        writer.writerow(
            {
                "user_id": record.user_id,
                "age": record.age,
                "country": record.country,
                "status": record.status,
                "amount": f"{record.amount:.2f}",
            }
        )
        count += 1
    return count


def run_etl(
    input_file: IO[str],
    output_file: IO[str],
    *,
    min_age: int = 18,
    min_amount: float = 100.0,
    countries: Iterable[str] = (),
) -> ETLSummary:
    """执行完整 ETL，并返回统计结果。"""

    summary = ETLSummary()
    country_set = {normalize_country(country) for country in countries if country.strip()}

    def filtered_records() -> Iterator[CleanRecord]:
        for row in extract(input_file):
            summary.read_rows += 1
            try:
                record = transform(row)
            except (KeyError, TypeError, ValueError):
                summary.invalid_rows += 1
                continue

            summary.valid_rows += 1
            if should_keep(record, min_age=min_age, min_amount=min_amount, countries=country_set):
                summary.filtered_rows += 1
                yield record

    load(filtered_records(), output_file)
    return summary


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    with open_input(args.input) as input_file, open_output(args.output) as output_file:
        summary = run_etl(
            input_file,
            output_file,
            min_age=args.min_age,
            min_amount=args.min_amount,
            countries=args.country,
        )
    print(
        "ETL summary: "
        f"read={summary.read_rows}, valid={summary.valid_rows}, "
        f"filtered={summary.filtered_rows}, invalid={summary.invalid_rows}",
        file=sys.stderr,
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
