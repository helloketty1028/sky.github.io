import csv
import io
import unittest

from big_data_filter_etl import CleanRecord, run_etl, should_keep, transform


SAMPLE_INPUT = """user_id,age,country,status,amount
u001,20,cn,active,120.5
u002,17,us,active,200
u003,35,cn,inactive,500
u004,48,US,VIP,99.99
u005,48,US,VIP,100
u006,28,jp,paid,300
u007,-1,cn,active,500
,30,cn,active,100
"""


class BigDataFilterETLTest(unittest.TestCase):
    def test_transform_normalizes_types_and_text(self):
        record = transform({"user_id": " u001 ", "age": "20", "country": " cn ", "status": " Active ", "amount": "120.5"})

        self.assertEqual(record, CleanRecord(user_id="u001", age=20, country="CN", status="active", amount=120.5))

    def test_should_keep_applies_business_filters(self):
        record = CleanRecord(user_id="u001", age=20, country="CN", status="active", amount=120.5)

        self.assertTrue(should_keep(record, min_age=18, min_amount=100, countries={"CN"}))
        self.assertFalse(should_keep(record, min_age=21, min_amount=100, countries={"CN"}))
        self.assertFalse(should_keep(record, min_age=18, min_amount=121, countries={"CN"}))
        self.assertFalse(should_keep(record, min_age=18, min_amount=100, countries={"US"}))

    def test_run_etl_filters_input_and_outputs_csv(self):
        input_file = io.StringIO(SAMPLE_INPUT)
        output_file = io.StringIO()

        summary = run_etl(input_file, output_file, min_age=18, min_amount=100, countries=["cn", "us"])

        self.assertEqual(summary.read_rows, 8)
        self.assertEqual(summary.valid_rows, 6)
        self.assertEqual(summary.invalid_rows, 2)
        self.assertEqual(summary.filtered_rows, 2)

        output_file.seek(0)
        rows = list(csv.DictReader(output_file))
        self.assertEqual(
            rows,
            [
                {"user_id": "u001", "age": "20", "country": "CN", "status": "active", "amount": "120.50"},
                {"user_id": "u005", "age": "48", "country": "US", "status": "vip", "amount": "100.00"},
            ],
        )


if __name__ == "__main__":
    unittest.main()
