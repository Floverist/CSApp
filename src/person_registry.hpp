#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include "person.hpp"



class PersonRegistry {
    std::unordered_map<int, Person> data;
public:
    bool insert(int id, const Person& person) {
        return data.emplace(id, person).second;
    }

    bool contains(int id) const {
        return data.find(id) != data.end();
    }

    Person& at(int id) { return data.at(id); }
    const Person& at(int id) const { return data.at(id); }

    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto size() const {return data.size();}

    void Print() const {
        std::cout << "Persons in registry:\n";
        for (const auto& [id, person] : data) {
            std::cout << "ID: " << id << ", Name: " << person.name << "\n";
        }
    }
};

#include <fstream>
#include <nlohmann/json.hpp>

class PersonRegistryLoader {
public:
    static bool LoadFromJson(const std::string& filename, PersonRegistry& registry) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        nlohmann::json j;
        try {
            file >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return false;
        }

        // Expected format: { "persons": [ { "id": 1, "name": "John" }, ... ] }
        if (!j.contains("persons") || !j["persons"].is_array()) {
            std::cerr << "Invalid JSON structure in file: " << filename << std::endl;
            return false;
        }

        for (const auto& p : j["persons"]) {
            if (!p.contains("id") || !p.contains("name")) {
                std::cerr << "Skipping invalid entry: missing id or name\n";
                continue;
            }

            int id = p["id"].get<int>();
            std::string card_number = p["card_number"].get<std::string>();
            std::string card_date = p["card_date"].get<std::string>();
            std::string name = p["name"].get<std::string>();
            std::cout << name;
            std::string unit = p["unit"].get<std::string>();
            std::string rank = p["rank"].get<std::string>();
            registry.insert(id, Person{ card_number,card_date,name, rank, unit });
        }

        return true;
    }
};
