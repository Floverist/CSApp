
#include <vector>
#include <algorithm>
#include <numeric>
#include "app-window.h"

#include"person_registry.hpp"


int main()
{
    PersonRegistry persons;
    PersonRegistryLoader::LoadFromJson("Resources/persons.json", persons);

    auto ui = MainWindow::create();

    std::vector<PersonData> all_data{};
    all_data.reserve(10000);

    for (auto& el: persons)
    {
        all_data.push_back(PersonData{
            slint::SharedString{el.second.card_number},
            slint::SharedString{el.second.card_date},
            slint::SharedString{el.second.name},
            slint::SharedString{el.second.rank},
            slint::SharedString{el.second.unit}
        });
    }

    std::vector<PersonData> filtered_data;    
    std::vector<size_t> filtered_indexes{};
    filtered_data.reserve(filtered_indexes.size());
    filtered_indexes.resize(all_data.size());
    std::iota(filtered_indexes.begin(), filtered_indexes.end(), 0);    

    auto model = std::make_shared<slint::VectorModel<PersonData>>(all_data);
    ui->set_rows(model);

    // UPDATE CELL (works on filtered view)
    ui->on_update_cell(
        [&filtered_indexes, &all_data, model](int row, int col, slint::SharedString value)
        {
            size_t index = filtered_indexes[row];
            auto item = all_data[index];

            model->set_row_data(row, item);
        });

    // FILTER
    ui->on_filter_changed(
        [&all_data, &filtered_indexes,&filtered_data, model]
        (slint::SharedString name,
        slint::SharedString rank,
        slint::SharedString unit)
        {
            filtered_indexes.clear();

            std::string n = name.data();
            std::string r = rank.data();
            std::string u = unit.data();


            size_t index = 0;
            for (const auto &row : all_data)
            {
                bool ok = true;

                // NAME filter
                if (!n.empty() &&
                    std::string(row.name).find(n) == std::string::npos)
                    ok = false;

                //RANK filter
                if (!r.empty() &&
                    std::string(row.rank).find(r) == std::string::npos)
                    ok = false;

                // // UNIT filter
                if (!u.empty() &&
                    std::string(row.unit).find(u) == std::string::npos)
                    ok = false;

                if (ok)
                    filtered_indexes.push_back(index);
                ++index;
            }

           
            for (size_t i : filtered_indexes)
                filtered_data.push_back(all_data[i]);
            model->set_vector(std::move(filtered_data));
    });

    ui->run();
}